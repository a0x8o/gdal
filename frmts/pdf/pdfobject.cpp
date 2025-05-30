/******************************************************************************
 *
 * Project:  PDF driver
 * Purpose:  GDALDataset driver for PDF dataset.
 * Author:   Even Rouault, <even dot rouault at spatialys.com>
 *
 ******************************************************************************
 *
 * Support for open-source PDFium library
 *
 * Copyright (C) 2015 Klokan Technologies GmbH (http://www.klokantech.com/)
 * Author: Martin Mikita <martin.mikita@klokantech.com>, xmikit00 @ FIT VUT Brno
 *
 ******************************************************************************
 * Copyright (c) 2011-2013, Even Rouault <even dot rouault at spatialys.com>
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#include "gdal_pdf.h"

#include <limits>
#include <vector>
#include "pdfobject.h"

/************************************************************************/
/*                        ROUND_IF_CLOSE()                       */
/************************************************************************/

double ROUND_IF_CLOSE(double x, double eps)
{
    if (eps == 0.0)
        eps = fabs(x) < 1 ? 1e-10 : 1e-8;
    const double dfRounded = std::round(x);
    if (fabs(x - dfRounded) < eps)
        return dfRounded;
    else
        return x;
}

/************************************************************************/
/*                         GDALPDFGetPDFString()                        */
/************************************************************************/

static CPLString GDALPDFGetPDFString(const char *pszStr)
{
    const GByte *pabyData = reinterpret_cast<const GByte *>(pszStr);
    GByte ch;
    for (size_t i = 0; (ch = pabyData[i]) != '\0'; i++)
    {
        if (ch < 32 || ch > 127 || ch == '(' || ch == ')' || ch == '\\' ||
            ch == '%' || ch == '#')
            break;
    }
    CPLString osStr;
    if (ch == 0)
    {
        osStr = "(";
        osStr += pszStr;
        osStr += ")";
        return osStr;
    }

    wchar_t *pwszDest = CPLRecodeToWChar(pszStr, CPL_ENC_UTF8, CPL_ENC_UCS2);
    osStr = "<FEFF";
    for (size_t i = 0; pwszDest[i] != 0; i++)
    {
#ifndef _WIN32
        if (pwszDest[i] >= 0x10000 /* && pwszDest[i] <= 0x10FFFF */)
        {
            /* Generate UTF-16 surrogate pairs (on Windows, CPLRecodeToWChar
             * does it for us)  */
            int nHeadSurrogate = ((pwszDest[i] - 0x10000) >> 10) | 0xd800;
            int nTrailSurrogate = ((pwszDest[i] - 0x10000) & 0x3ff) | 0xdc00;
            osStr += CPLSPrintf("%02X", (nHeadSurrogate >> 8) & 0xff);
            osStr += CPLSPrintf("%02X", (nHeadSurrogate)&0xff);
            osStr += CPLSPrintf("%02X", (nTrailSurrogate >> 8) & 0xff);
            osStr += CPLSPrintf("%02X", (nTrailSurrogate)&0xff);
        }
        else
#endif
        {
            osStr +=
                CPLSPrintf("%02X", static_cast<int>(pwszDest[i] >> 8) & 0xff);
            osStr += CPLSPrintf("%02X", static_cast<int>(pwszDest[i]) & 0xff);
        }
    }
    osStr += ">";
    CPLFree(pwszDest);
    return osStr;
}

#if defined(HAVE_POPPLER) || defined(HAVE_PDFIUM)

/************************************************************************/
/*                     GDALPDFGetUTF8StringFromBytes()                  */
/************************************************************************/

static std::string GDALPDFGetUTF8StringFromBytes(const GByte *pabySrc,
                                                 size_t nLen)
{
    const bool bLEUnicodeMarker =
        nLen >= 2 && pabySrc[0] == 0xFE && pabySrc[1] == 0xFF;
    const bool bBEUnicodeMarker =
        nLen >= 2 && pabySrc[0] == 0xFF && pabySrc[1] == 0xFE;
    if (!bLEUnicodeMarker && !bBEUnicodeMarker)
    {
        std::string osStr;
        try
        {
            osStr.reserve(nLen);
        }
        catch (const std::exception &e)
        {
            CPLError(CE_Failure, CPLE_OutOfMemory,
                     "GDALPDFGetUTF8StringFromBytes(): %s", e.what());
            return osStr;
        }
        osStr.assign(reinterpret_cast<const char *>(pabySrc), nLen);
        const char *pszStr = osStr.c_str();
        if (CPLIsUTF8(pszStr, -1))
            return osStr;
        else
        {
            char *pszUTF8 = CPLRecode(pszStr, CPL_ENC_ISO8859_1, CPL_ENC_UTF8);
            std::string osRet = pszUTF8;
            CPLFree(pszUTF8);
            return osRet;
        }
    }

    /* This is UTF-16 content */
    pabySrc += 2;
    nLen = (nLen - 2) / 2;
    std::wstring awszSource;
    awszSource.resize(nLen + 1);
    size_t j = 0;
    for (size_t i = 0; i < nLen; i++, j++)
    {
        if (!bBEUnicodeMarker)
            awszSource[j] = (pabySrc[2 * i] << 8) + pabySrc[2 * i + 1];
        else
            awszSource[j] = (pabySrc[2 * i + 1] << 8) + pabySrc[2 * i];
#ifndef _WIN32
        /* Is there a surrogate pair ? See http://en.wikipedia.org/wiki/UTF-16
         */
        /* On Windows, CPLRecodeFromWChar does this for us, because wchar_t is
         * only */
        /* 2 bytes wide, whereas on Unix it is 32bits */
        if (awszSource[j] >= 0xD800 && awszSource[j] <= 0xDBFF && i + 1 < nLen)
        {
            /* should be in the range 0xDC00... 0xDFFF */
            wchar_t nTrailSurrogate;
            if (!bBEUnicodeMarker)
                nTrailSurrogate =
                    (pabySrc[2 * (i + 1)] << 8) + pabySrc[2 * (i + 1) + 1];
            else
                nTrailSurrogate =
                    (pabySrc[2 * (i + 1) + 1] << 8) + pabySrc[2 * (i + 1)];
            if (nTrailSurrogate >= 0xDC00 && nTrailSurrogate <= 0xDFFF)
            {
                awszSource[j] = ((awszSource[j] - 0xD800) << 10) +
                                (nTrailSurrogate - 0xDC00) + 0x10000;
                i++;
            }
        }
#endif
    }
    awszSource[j] = 0;

    char *pszUTF8 =
        CPLRecodeFromWChar(awszSource.data(), CPL_ENC_UCS2, CPL_ENC_UTF8);
    awszSource.clear();
    std::string osStrUTF8(pszUTF8);
    CPLFree(pszUTF8);
    return osStrUTF8;
}

#endif  // defined(HAVE_POPPLER) || defined(HAVE_PDFIUM)

/************************************************************************/
/*                          GDALPDFGetPDFName()                         */
/************************************************************************/

static std::string GDALPDFGetPDFName(const std::string &osStr)
{
    std::string osRet;
    for (const char ch : osStr)
    {
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
              (ch >= '0' && ch <= '9') || ch == '-'))
            osRet += '_';
        else
            osRet += ch;
    }
    return osRet;
}

/************************************************************************/
/* ==================================================================== */
/*                            GDALPDFObject                             */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                            ~GDALPDFObject()                          */
/************************************************************************/

GDALPDFObject::~GDALPDFObject()
{
}

/************************************************************************/
/*                            LookupObject()                            */
/************************************************************************/

GDALPDFObject *GDALPDFObject::LookupObject(const char *pszPath)
{
    if (GetType() != PDFObjectType_Dictionary)
        return nullptr;
    return GetDictionary()->LookupObject(pszPath);
}

/************************************************************************/
/*                             GetTypeName()                            */
/************************************************************************/

const char *GDALPDFObject::GetTypeName()
{
    switch (GetType())
    {
        case PDFObjectType_Unknown:
            return GetTypeNameNative();
        case PDFObjectType_Null:
            return "null";
        case PDFObjectType_Bool:
            return "bool";
        case PDFObjectType_Int:
            return "int";
        case PDFObjectType_Real:
            return "real";
        case PDFObjectType_String:
            return "string";
        case PDFObjectType_Name:
            return "name";
        case PDFObjectType_Array:
            return "array";
        case PDFObjectType_Dictionary:
            return "dictionary";
        default:
            return GetTypeNameNative();
    }
}

/************************************************************************/
/*                             Serialize()                              */
/************************************************************************/

void GDALPDFObject::Serialize(CPLString &osStr, bool bEmitRef)
{
    auto nRefNum = GetRefNum();
    if (bEmitRef && nRefNum.toBool())
    {
        int nRefGen = GetRefGen();
        osStr.append(CPLSPrintf("%d %d R", nRefNum.toInt(), nRefGen));
        return;
    }

    switch (GetType())
    {
        case PDFObjectType_Null:
            osStr.append("null");
            return;
        case PDFObjectType_Bool:
            osStr.append(GetBool() ? "true" : "false");
            return;
        case PDFObjectType_Int:
            osStr.append(CPLSPrintf("%d", GetInt()));
            return;
        case PDFObjectType_Real:
        {
            char szReal[512];
            double dfRealNonRounded = GetReal();
            double dfReal = ROUND_IF_CLOSE(dfRealNonRounded);
            if (dfReal >=
                    static_cast<double>(std::numeric_limits<GIntBig>::min()) &&
                dfReal <=
                    static_cast<double>(std::numeric_limits<GIntBig>::max()) &&
                dfReal == static_cast<double>(static_cast<GIntBig>(dfReal)))
            {
                snprintf(szReal, sizeof(szReal), CPL_FRMT_GIB,
                         static_cast<GIntBig>(dfReal));
            }
            else if (CanRepresentRealAsString())
            {
                /* Used for OGC BP numeric values */
                CPLsnprintf(szReal, sizeof(szReal), "(%.*g)", GetPrecision(),
                            dfReal);
            }
            else
            {
                CPLsnprintf(szReal, sizeof(szReal), "%.*f", GetPrecision(),
                            dfReal);

                /* Remove non significant trailing zeroes */
                char *pszDot = strchr(szReal, '.');
                if (pszDot)
                {
                    int iDot = static_cast<int>(pszDot - szReal);
                    int nLen = static_cast<int>(strlen(szReal));
                    for (int i = nLen - 1; i > iDot; i--)
                    {
                        if (szReal[i] == '0')
                            szReal[i] = '\0';
                        else
                            break;
                    }
                }
            }
            osStr.append(szReal);
            return;
        }
        case PDFObjectType_String:
            osStr.append(GDALPDFGetPDFString(GetString().c_str()));
            return;
        case PDFObjectType_Name:
            osStr.append("/");
            osStr.append(GDALPDFGetPDFName(GetName()));
            return;
        case PDFObjectType_Array:
            GetArray()->Serialize(osStr);
            return;
        case PDFObjectType_Dictionary:
            GetDictionary()->Serialize(osStr);
            return;
        case PDFObjectType_Unknown:
        default:
            CPLError(CE_Warning, CPLE_AppDefined,
                     "Serializing unknown object !");
            return;
    }
}

/************************************************************************/
/*                               Clone()                                */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObject::Clone()
{
    auto nRefNum = GetRefNum();
    if (nRefNum.toBool())
    {
        int nRefGen = GetRefGen();
        return GDALPDFObjectRW::CreateIndirect(nRefNum, nRefGen);
    }

    switch (GetType())
    {
        case PDFObjectType_Null:
            return GDALPDFObjectRW::CreateNull();
        case PDFObjectType_Bool:
            return GDALPDFObjectRW::CreateBool(GetBool());
        case PDFObjectType_Int:
            return GDALPDFObjectRW::CreateInt(GetInt());
        case PDFObjectType_Real:
            return GDALPDFObjectRW::CreateReal(GetReal());
        case PDFObjectType_String:
            return GDALPDFObjectRW::CreateString(GetString().c_str());
        case PDFObjectType_Name:
            return GDALPDFObjectRW::CreateName(GetName().c_str());
        case PDFObjectType_Array:
            return GDALPDFObjectRW::CreateArray(GetArray()->Clone());
        case PDFObjectType_Dictionary:
            return GDALPDFObjectRW::CreateDictionary(GetDictionary()->Clone());
        case PDFObjectType_Unknown:
        default:
            CPLError(CE_Warning, CPLE_AppDefined, "Cloning unknown object !");
            return nullptr;
    }
}

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFDictionary                            */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                        ~GDALPDFDictionary()                          */
/************************************************************************/

GDALPDFDictionary::~GDALPDFDictionary()
{
}

/************************************************************************/
/*                            LookupObject()                            */
/************************************************************************/

GDALPDFObject *GDALPDFDictionary::LookupObject(const char *pszPath)
{
    GDALPDFObject *poCurObj = nullptr;
    char **papszTokens = CSLTokenizeString2(pszPath, ".", 0);
    for (int i = 0; papszTokens[i] != nullptr; i++)
    {
        int iElt = -1;
        char *pszBracket = strchr(papszTokens[i], '[');
        if (pszBracket != nullptr)
        {
            iElt = atoi(pszBracket + 1);
            *pszBracket = '\0';
        }

        if (i == 0)
        {
            poCurObj = Get(papszTokens[i]);
        }
        else
        {
            if (poCurObj->GetType() != PDFObjectType_Dictionary)
            {
                poCurObj = nullptr;
                break;
            }
            poCurObj = poCurObj->GetDictionary()->Get(papszTokens[i]);
        }

        if (poCurObj == nullptr)
        {
            poCurObj = nullptr;
            break;
        }

        if (iElt >= 0)
        {
            if (poCurObj->GetType() != PDFObjectType_Array)
            {
                poCurObj = nullptr;
                break;
            }
            poCurObj = poCurObj->GetArray()->Get(iElt);
        }
    }
    CSLDestroy(papszTokens);
    return poCurObj;
}

/************************************************************************/
/*                             Serialize()                              */
/************************************************************************/

void GDALPDFDictionary::Serialize(CPLString &osStr)
{
    osStr.append("<< ");
    for (const auto &oIter : GetValues())
    {
        const char *pszKey = oIter.first.c_str();
        GDALPDFObject *poObj = oIter.second;
        osStr.append("/");
        osStr.append(pszKey);
        osStr.append(" ");
        poObj->Serialize(osStr);
        osStr.append(" ");
    }
    osStr.append(">>");
}

/************************************************************************/
/*                               Clone()                                */
/************************************************************************/

GDALPDFDictionaryRW *GDALPDFDictionary::Clone()
{
    GDALPDFDictionaryRW *poDict = new GDALPDFDictionaryRW();
    for (const auto &oIter : GetValues())
    {
        const char *pszKey = oIter.first.c_str();
        GDALPDFObject *poObj = oIter.second;
        poDict->Add(pszKey, poObj->Clone());
    }
    return poDict;
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFArray                               */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                           ~GDALPDFArray()                            */
/************************************************************************/

GDALPDFArray::~GDALPDFArray()
{
}

/************************************************************************/
/*                             Serialize()                              */
/************************************************************************/

void GDALPDFArray::Serialize(CPLString &osStr)
{
    int nLength = GetLength();
    int i;

    osStr.append("[ ");
    for (i = 0; i < nLength; i++)
    {
        Get(i)->Serialize(osStr);
        osStr.append(" ");
    }
    osStr.append("]");
}

/************************************************************************/
/*                               Clone()                                */
/************************************************************************/

GDALPDFArrayRW *GDALPDFArray::Clone()
{
    GDALPDFArrayRW *poArray = new GDALPDFArrayRW();
    int nLength = GetLength();
    int i;
    for (i = 0; i < nLength; i++)
    {
        poArray->Add(Get(i)->Clone());
    }
    return poArray;
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFStream                              */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                           ~GDALPDFStream()                           */
/************************************************************************/

GDALPDFStream::~GDALPDFStream() = default;

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFObjectRW                            */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                           GDALPDFObjectRW()                          */
/************************************************************************/

GDALPDFObjectRW::GDALPDFObjectRW(GDALPDFObjectType eType) : m_eType(eType)
{
}

/************************************************************************/
/*                             ~GDALPDFObjectRW()                       */
/************************************************************************/

GDALPDFObjectRW::~GDALPDFObjectRW()
{
    delete m_poDict;
    delete m_poArray;
}

/************************************************************************/
/*                            CreateIndirect()                          */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateIndirect(const GDALPDFObjectNum &nNum,
                                                 int nGen)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Unknown);
    poObj->m_nNum = nNum;
    poObj->m_nGen = nGen;
    return poObj;
}

/************************************************************************/
/*                             CreateNull()                             */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateNull()
{
    return new GDALPDFObjectRW(PDFObjectType_Null);
}

/************************************************************************/
/*                             CreateBool()                             */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateBool(int bVal)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Bool);
    poObj->m_nVal = bVal;
    return poObj;
}

/************************************************************************/
/*                             CreateInt()                              */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateInt(int nVal)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Int);
    poObj->m_nVal = nVal;
    return poObj;
}

/************************************************************************/
/*                            CreateReal()                              */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateReal(double dfVal,
                                             int bCanRepresentRealAsString)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Real);
    poObj->m_dfVal = dfVal;
    poObj->m_bCanRepresentRealAsString = bCanRepresentRealAsString;
    return poObj;
}

/************************************************************************/
/*                       CreateRealWithPrecision()                      */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateRealWithPrecision(double dfVal,
                                                          int nPrecision)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Real);
    poObj->m_dfVal = dfVal;
    poObj->m_nPrecision = nPrecision;
    return poObj;
}

/************************************************************************/
/*                           CreateString()                             */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateString(const char *pszStr)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_String);
    poObj->m_osVal = pszStr;
    return poObj;
}

/************************************************************************/
/*                            CreateName()                              */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateName(const char *pszName)
{
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Name);
    poObj->m_osVal = pszName;
    return poObj;
}

/************************************************************************/
/*                          CreateDictionary()                          */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateDictionary(GDALPDFDictionaryRW *poDict)
{
    CPLAssert(poDict);
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Dictionary);
    poObj->m_poDict = poDict;
    return poObj;
}

/************************************************************************/
/*                            CreateArray()                             */
/************************************************************************/

GDALPDFObjectRW *GDALPDFObjectRW::CreateArray(GDALPDFArrayRW *poArray)
{
    CPLAssert(poArray);
    GDALPDFObjectRW *poObj = new GDALPDFObjectRW(PDFObjectType_Array);
    poObj->m_poArray = poArray;
    return poObj;
}

/************************************************************************/
/*                          GetTypeNameNative()                         */
/************************************************************************/

const char *GDALPDFObjectRW::GetTypeNameNative()
{
    CPLError(CE_Failure, CPLE_AppDefined, "Should not go here");
    return "";
}

/************************************************************************/
/*                             GetType()                                */
/************************************************************************/

GDALPDFObjectType GDALPDFObjectRW::GetType()
{
    return m_eType;
}

/************************************************************************/
/*                             GetBool()                                */
/************************************************************************/

int GDALPDFObjectRW::GetBool()
{
    if (m_eType == PDFObjectType_Bool)
        return m_nVal;

    return FALSE;
}

/************************************************************************/
/*                               GetInt()                               */
/************************************************************************/

int GDALPDFObjectRW::GetInt()
{
    if (m_eType == PDFObjectType_Int)
        return m_nVal;

    return 0;
}

/************************************************************************/
/*                              GetReal()                               */
/************************************************************************/

double GDALPDFObjectRW::GetReal()
{
    return m_dfVal;
}

/************************************************************************/
/*                             GetString()                              */
/************************************************************************/

const CPLString &GDALPDFObjectRW::GetString()
{
    return m_osVal;
}

/************************************************************************/
/*                              GetName()                               */
/************************************************************************/

const CPLString &GDALPDFObjectRW::GetName()
{
    return m_osVal;
}

/************************************************************************/
/*                            GetDictionary()                           */
/************************************************************************/

GDALPDFDictionary *GDALPDFObjectRW::GetDictionary()
{
    return m_poDict;
}

/************************************************************************/
/*                              GetArray()                              */
/************************************************************************/

GDALPDFArray *GDALPDFObjectRW::GetArray()
{
    return m_poArray;
}

/************************************************************************/
/*                              GetStream()                             */
/************************************************************************/

GDALPDFStream *GDALPDFObjectRW::GetStream()
{
    return nullptr;
}

/************************************************************************/
/*                              GetRefNum()                             */
/************************************************************************/

GDALPDFObjectNum GDALPDFObjectRW::GetRefNum()
{
    return m_nNum;
}

/************************************************************************/
/*                              GetRefGen()                             */
/************************************************************************/

int GDALPDFObjectRW::GetRefGen()
{
    return m_nGen;
}

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFDictionaryRW                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                           GDALPDFDictionaryRW()                      */
/************************************************************************/

GDALPDFDictionaryRW::GDALPDFDictionaryRW() = default;

/************************************************************************/
/*                          ~GDALPDFDictionaryRW()                      */
/************************************************************************/

GDALPDFDictionaryRW::~GDALPDFDictionaryRW()
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.begin();
    std::map<CPLString, GDALPDFObject *>::iterator oEnd = m_map.end();
    for (; oIter != oEnd; ++oIter)
        delete oIter->second;
}

/************************************************************************/
/*                                   Get()                              */
/************************************************************************/

GDALPDFObject *GDALPDFDictionaryRW::Get(const char *pszKey)
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.find(pszKey);
    if (oIter != m_map.end())
        return oIter->second;
    return nullptr;
}

/************************************************************************/
/*                               GetValues()                            */
/************************************************************************/

std::map<CPLString, GDALPDFObject *> &GDALPDFDictionaryRW::GetValues()
{
    return m_map;
}

/************************************************************************/
/*                                 Add()                                */
/************************************************************************/

GDALPDFDictionaryRW &GDALPDFDictionaryRW::Add(const char *pszKey,
                                              GDALPDFObject *poVal)
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.find(pszKey);
    if (oIter != m_map.end())
    {
        delete oIter->second;
        oIter->second = poVal;
    }
    else
        m_map[pszKey] = poVal;

    return *this;
}

/************************************************************************/
/*                                Remove()                              */
/************************************************************************/

GDALPDFDictionaryRW &GDALPDFDictionaryRW::Remove(const char *pszKey)
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.find(pszKey);
    if (oIter != m_map.end())
    {
        delete oIter->second;
        m_map.erase(pszKey);
    }

    return *this;
}

/************************************************************************/
/* ==================================================================== */
/*                            GDALPDFArrayRW                            */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                             GDALPDFArrayRW()                         */
/************************************************************************/

GDALPDFArrayRW::GDALPDFArrayRW()
{
}

/************************************************************************/
/*                            ~GDALPDFArrayRW()                         */
/************************************************************************/

GDALPDFArrayRW::~GDALPDFArrayRW()
{
    for (size_t i = 0; i < m_array.size(); i++)
        delete m_array[i];
}

/************************************************************************/
/*                               GetLength()                             */
/************************************************************************/

int GDALPDFArrayRW::GetLength()
{
    return static_cast<int>(m_array.size());
}

/************************************************************************/
/*                                  Get()                               */
/************************************************************************/

GDALPDFObject *GDALPDFArrayRW::Get(int nIndex)
{
    if (nIndex < 0 || nIndex >= GetLength())
        return nullptr;
    return m_array[nIndex];
}

/************************************************************************/
/*                                  Add()                               */
/************************************************************************/

GDALPDFArrayRW &GDALPDFArrayRW::Add(GDALPDFObject *poObj)
{
    m_array.push_back(poObj);
    return *this;
}

/************************************************************************/
/*                                  Add()                               */
/************************************************************************/

GDALPDFArrayRW &GDALPDFArrayRW::Add(double *padfVal, int nCount,
                                    int bCanRepresentRealAsString)
{
    for (int i = 0; i < nCount; i++)
        m_array.push_back(
            GDALPDFObjectRW::CreateReal(padfVal[i], bCanRepresentRealAsString));
    return *this;
}

#ifdef HAVE_POPPLER

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFDictionaryPoppler                     */
/* ==================================================================== */
/************************************************************************/

class GDALPDFDictionaryPoppler : public GDALPDFDictionary
{
  private:
    Dict *m_poDict;
    std::map<CPLString, GDALPDFObject *> m_map{};

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFDictionaryPoppler)

  public:
    GDALPDFDictionaryPoppler(Dict *poDict) : m_poDict(poDict)
    {
    }

    virtual ~GDALPDFDictionaryPoppler();

    virtual GDALPDFObject *Get(const char *pszKey) override;
    virtual std::map<CPLString, GDALPDFObject *> &GetValues() override;
};

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFArrayPoppler                        */
/* ==================================================================== */
/************************************************************************/

class GDALPDFArrayPoppler : public GDALPDFArray
{
  private:
    const Array *m_poArray;
    std::vector<std::unique_ptr<GDALPDFObject>> m_v{};

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFArrayPoppler)

  public:
    GDALPDFArrayPoppler(const Array *poArray) : m_poArray(poArray)
    {
    }

    virtual int GetLength() override;
    virtual GDALPDFObject *Get(int nIndex) override;
};

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFStreamPoppler                       */
/* ==================================================================== */
/************************************************************************/

class GDALPDFStreamPoppler : public GDALPDFStream
{
  private:
    int64_t m_nLength = -1;
    Stream *m_poStream;
    int64_t m_nRawLength = -1;

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFStreamPoppler)

  public:
    GDALPDFStreamPoppler(Stream *poStream) : m_poStream(poStream)
    {
    }

    virtual int64_t GetLength(int64_t nMaxSize = 0) override;
    virtual char *GetBytes() override;

    virtual int64_t GetRawLength() override;
    virtual char *GetRawBytes() override;
};

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFObjectPoppler                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                          ~GDALPDFObjectPoppler()                     */
/************************************************************************/

GDALPDFObjectPoppler::~GDALPDFObjectPoppler()
{
    delete m_poToDestroy;
    delete m_poDict;
    delete m_poArray;
    delete m_poStream;
}

/************************************************************************/
/*                              GetType()                               */
/************************************************************************/

GDALPDFObjectType GDALPDFObjectPoppler::GetType()
{
    switch (m_poConst->getType())
    {
        case objNull:
            return PDFObjectType_Null;
        case objBool:
            return PDFObjectType_Bool;
        case objInt:
            return PDFObjectType_Int;
        case objReal:
            return PDFObjectType_Real;
        case objString:
            return PDFObjectType_String;
        case objName:
            return PDFObjectType_Name;
        case objArray:
            return PDFObjectType_Array;
        case objDict:
            return PDFObjectType_Dictionary;
        case objStream:
            return PDFObjectType_Dictionary;
        default:
            return PDFObjectType_Unknown;
    }
}

/************************************************************************/
/*                          GetTypeNameNative()                         */
/************************************************************************/

const char *GDALPDFObjectPoppler::GetTypeNameNative()
{
    return m_poConst->getTypeName();
}

/************************************************************************/
/*                               GetBool()                              */
/************************************************************************/

int GDALPDFObjectPoppler::GetBool()
{
    if (GetType() == PDFObjectType_Bool)
        return m_poConst->getBool();
    else
        return 0;
}

/************************************************************************/
/*                               GetInt()                               */
/************************************************************************/

int GDALPDFObjectPoppler::GetInt()
{
    if (GetType() == PDFObjectType_Int)
        return m_poConst->getInt();
    else
        return 0;
}

/************************************************************************/
/*                               GetReal()                              */
/************************************************************************/

double GDALPDFObjectPoppler::GetReal()
{
    if (GetType() == PDFObjectType_Real)
        return m_poConst->getReal();
    else
        return 0.0;
}

/************************************************************************/
/*                              GetString()                             */
/************************************************************************/

const std::string &GDALPDFObjectPoppler::GetString()
{
    if (GetType() == PDFObjectType_String)
    {
        const GooString *gooString = m_poConst->getString();
        const std::string &osStdStr = gooString->toStr();
        const bool bLEUnicodeMarker =
            osStdStr.size() >= 2 && static_cast<uint8_t>(osStdStr[0]) == 0xFE &&
            static_cast<uint8_t>(osStdStr[1]) == 0xFF;
        const bool bBEUnicodeMarker =
            osStdStr.size() >= 2 && static_cast<uint8_t>(osStdStr[0]) == 0xFF &&
            static_cast<uint8_t>(osStdStr[1]) == 0xFE;
        if (!bLEUnicodeMarker && !bBEUnicodeMarker)
        {
            if (CPLIsUTF8(osStdStr.c_str(), -1))
            {
                return osStdStr;
            }
            else
            {
                char *pszUTF8 =
                    CPLRecode(osStdStr.data(), CPL_ENC_ISO8859_1, CPL_ENC_UTF8);
                osStr = pszUTF8;
                CPLFree(pszUTF8);
                return osStr;
            }
        }
        return (osStr = GDALPDFGetUTF8StringFromBytes(
                    reinterpret_cast<const GByte *>(osStdStr.data()),
                    osStdStr.size()));
    }
    else
        return (osStr = "");
}

/************************************************************************/
/*                               GetName()                              */
/************************************************************************/

const std::string &GDALPDFObjectPoppler::GetName()
{
    if (GetType() == PDFObjectType_Name)
        return (osStr = m_poConst->getName());
    else
        return (osStr = "");
}

/************************************************************************/
/*                            GetDictionary()                           */
/************************************************************************/

GDALPDFDictionary *GDALPDFObjectPoppler::GetDictionary()
{
    if (GetType() != PDFObjectType_Dictionary)
        return nullptr;

    if (m_poDict)
        return m_poDict;

    Dict *poDict = (m_poConst->getType() == objStream)
                       ? m_poConst->getStream()->getDict()
                       : m_poConst->getDict();
    if (poDict == nullptr)
        return nullptr;
    m_poDict = new GDALPDFDictionaryPoppler(poDict);
    return m_poDict;
}

/************************************************************************/
/*                              GetArray()                              */
/************************************************************************/

GDALPDFArray *GDALPDFObjectPoppler::GetArray()
{
    if (GetType() != PDFObjectType_Array)
        return nullptr;

    if (m_poArray)
        return m_poArray;

    Array *poArray = m_poConst->getArray();
    if (poArray == nullptr)
        return nullptr;
    m_poArray = new GDALPDFArrayPoppler(poArray);
    return m_poArray;
}

/************************************************************************/
/*                             GetStream()                              */
/************************************************************************/

GDALPDFStream *GDALPDFObjectPoppler::GetStream()
{
    if (m_poConst->getType() != objStream)
        return nullptr;

    if (m_poStream)
        return m_poStream;
    m_poStream = new GDALPDFStreamPoppler(m_poConst->getStream());
    return m_poStream;
}

/************************************************************************/
/*                           SetRefNumAndGen()                          */
/************************************************************************/

void GDALPDFObjectPoppler::SetRefNumAndGen(const GDALPDFObjectNum &nNum,
                                           int nGen)
{
    m_nRefNum = nNum;
    m_nRefGen = nGen;
}

/************************************************************************/
/*                               GetRefNum()                            */
/************************************************************************/

GDALPDFObjectNum GDALPDFObjectPoppler::GetRefNum()
{
    return m_nRefNum;
}

/************************************************************************/
/*                               GetRefGen()                            */
/************************************************************************/

int GDALPDFObjectPoppler::GetRefGen()
{
    return m_nRefGen;
}

/************************************************************************/
/* ==================================================================== */
/*                        GDALPDFDictionaryPoppler                      */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                       ~GDALPDFDictionaryPoppler()                    */
/************************************************************************/

GDALPDFDictionaryPoppler::~GDALPDFDictionaryPoppler()
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.begin();
    std::map<CPLString, GDALPDFObject *>::iterator oEnd = m_map.end();
    for (; oIter != oEnd; ++oIter)
        delete oIter->second;
}

/************************************************************************/
/*                                  Get()                               */
/************************************************************************/

GDALPDFObject *GDALPDFDictionaryPoppler::Get(const char *pszKey)
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.find(pszKey);
    if (oIter != m_map.end())
        return oIter->second;

    auto &&o(m_poDict->lookupNF(pszKey));
    if (!o.isNull())
    {
        GDALPDFObjectNum nRefNum;
        int nRefGen = 0;
        if (o.isRef())
        {
            nRefNum = o.getRefNum();
            nRefGen = o.getRefGen();
            Object o2(m_poDict->lookup(pszKey));
            if (!o2.isNull())
            {
                GDALPDFObjectPoppler *poObj =
                    new GDALPDFObjectPoppler(new Object(std::move(o2)), TRUE);
                poObj->SetRefNumAndGen(nRefNum, nRefGen);
                m_map[pszKey] = poObj;
                return poObj;
            }
        }
        else
        {
            GDALPDFObjectPoppler *poObj =
                new GDALPDFObjectPoppler(new Object(o.copy()), TRUE);
            poObj->SetRefNumAndGen(nRefNum, nRefGen);
            m_map[pszKey] = poObj;
            return poObj;
        }
    }
    return nullptr;
}

/************************************************************************/
/*                                GetValues()                           */
/************************************************************************/

std::map<CPLString, GDALPDFObject *> &GDALPDFDictionaryPoppler::GetValues()
{
    int i = 0;
    int nLength = m_poDict->getLength();
    for (i = 0; i < nLength; i++)
    {
        const char *pszKey = m_poDict->getKey(i);
        Get(pszKey);
    }
    return m_map;
}

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFArrayPoppler                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                           GDALPDFCreateArray()                       */
/************************************************************************/

GDALPDFArray *GDALPDFCreateArray(const Array *array)
{
    return new GDALPDFArrayPoppler(array);
}

/************************************************************************/
/*                               GetLength()                            */
/************************************************************************/

int GDALPDFArrayPoppler::GetLength()
{
    return m_poArray->getLength();
}

/************************************************************************/
/*                                 Get()                                */
/************************************************************************/

GDALPDFObject *GDALPDFArrayPoppler::Get(int nIndex)
{
    if (nIndex < 0 || nIndex >= GetLength())
        return nullptr;

    if (m_v.empty())
        m_v.resize(GetLength());

    if (m_v[nIndex] != nullptr)
        return m_v[nIndex].get();

    auto &&o(m_poArray->getNF(nIndex));
    if (!o.isNull())
    {
        GDALPDFObjectNum nRefNum;
        int nRefGen = 0;
        if (o.isRef())
        {
            nRefNum = o.getRefNum();
            nRefGen = o.getRefGen();
            Object o2(m_poArray->get(nIndex));
            if (!o2.isNull())
            {
                auto poObj = std::make_unique<GDALPDFObjectPoppler>(
                    new Object(std::move(o2)), TRUE);
                poObj->SetRefNumAndGen(nRefNum, nRefGen);
                m_v[nIndex] = std::move(poObj);
                return m_v[nIndex].get();
            }
        }
        else
        {
            auto poObj = std::make_unique<GDALPDFObjectPoppler>(
                new Object(o.copy()), TRUE);
            poObj->SetRefNumAndGen(nRefNum, nRefGen);
            m_v[nIndex] = std::move(poObj);
            return m_v[nIndex].get();
        }
    }
    return nullptr;
}

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFStreamPoppler                        */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                               GetLength()                            */
/************************************************************************/

int64_t GDALPDFStreamPoppler::GetLength(int64_t nMaxSize)
{
    if (m_nLength >= 0)
        return m_nLength;

#if POPPLER_MAJOR_VERSION > 25 ||                                              \
    (POPPLER_MAJOR_VERSION == 25 && POPPLER_MINOR_VERSION >= 2)
    if (!m_poStream->reset())
        return 0;
#else
    m_poStream->reset();
#endif
    m_nLength = 0;
    unsigned char readBuf[4096];
    int readChars;
    while ((readChars = m_poStream->doGetChars(4096, readBuf)) != 0)
    {
        m_nLength += readChars;
        if (nMaxSize != 0 && m_nLength > nMaxSize)
        {
            m_nLength = -1;
            return std::numeric_limits<int64_t>::max();
        }
    }
    return m_nLength;
}

/************************************************************************/
/*                         GooStringToCharStart()                       */
/************************************************************************/

static char *GooStringToCharStart(GooString &gstr)
{
    auto nLength = gstr.getLength();
    if (nLength)
    {
        char *pszContent = static_cast<char *>(VSI_MALLOC_VERBOSE(nLength + 1));
        if (pszContent)
        {
            const char *srcStr = gstr.c_str();
            memcpy(pszContent, srcStr, nLength);
            pszContent[nLength] = '\0';
        }
        return pszContent;
    }
    return nullptr;
}

/************************************************************************/
/*                               GetBytes()                             */
/************************************************************************/

char *GDALPDFStreamPoppler::GetBytes()
{
    GooString gstr;
    try
    {
        m_poStream->fillGooString(&gstr);
    }
    catch (const std::exception &e)
    {
        CPLError(CE_Failure, CPLE_OutOfMemory,
                 "GDALPDFStreamPoppler::GetBytes(): %s", e.what());
        return nullptr;
    }
    m_nLength = static_cast<int64_t>(gstr.toStr().size());
    return GooStringToCharStart(gstr);
}

/************************************************************************/
/*                            GetRawLength()                            */
/************************************************************************/

int64_t GDALPDFStreamPoppler::GetRawLength()
{
    if (m_nRawLength >= 0)
        return m_nRawLength;

    auto undecodeStream = m_poStream->getUndecodedStream();
#if POPPLER_MAJOR_VERSION > 25 ||                                              \
    (POPPLER_MAJOR_VERSION == 25 && POPPLER_MINOR_VERSION >= 2)
    if (!undecodeStream->reset())
        return 0;
#else
    undecodeStream->reset();
#endif
    m_nRawLength = 0;
    while (undecodeStream->getChar() != EOF)
        m_nRawLength++;
    return m_nRawLength;
}

/************************************************************************/
/*                             GetRawBytes()                            */
/************************************************************************/

char *GDALPDFStreamPoppler::GetRawBytes()
{
    GooString gstr;
    auto undecodeStream = m_poStream->getUndecodedStream();
    try
    {
        undecodeStream->fillGooString(&gstr);
    }
    catch (const std::exception &e)
    {
        CPLError(CE_Failure, CPLE_OutOfMemory,
                 "GDALPDFStreamPoppler::GetRawBytes(): %s", e.what());
        return nullptr;
    }
    m_nRawLength = gstr.getLength();
    return GooStringToCharStart(gstr);
}

#endif  // HAVE_POPPLER

#ifdef HAVE_PODOFO

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFDictionaryPodofo                      */
/* ==================================================================== */
/************************************************************************/

class GDALPDFDictionaryPodofo : public GDALPDFDictionary
{
  private:
    const PoDoFo::PdfDictionary *m_poDict;
    const PoDoFo::PdfVecObjects &m_poObjects;
    std::map<CPLString, GDALPDFObject *> m_map{};

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFDictionaryPodofo)

  public:
    GDALPDFDictionaryPodofo(const PoDoFo::PdfDictionary *poDict,
                            const PoDoFo::PdfVecObjects &poObjects)
        : m_poDict(poDict), m_poObjects(poObjects)
    {
    }

    virtual ~GDALPDFDictionaryPodofo();

    virtual GDALPDFObject *Get(const char *pszKey) override;
    virtual std::map<CPLString, GDALPDFObject *> &GetValues() override;
};

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFArrayPodofo                         */
/* ==================================================================== */
/************************************************************************/

class GDALPDFArrayPodofo : public GDALPDFArray
{
  private:
    const PoDoFo::PdfArray *m_poArray;
    const PoDoFo::PdfVecObjects &m_poObjects;
    std::vector<std::unique_ptr<GDALPDFObject>> m_v{};

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFArrayPodofo)

  public:
    GDALPDFArrayPodofo(const PoDoFo::PdfArray *poArray,
                       const PoDoFo::PdfVecObjects &poObjects)
        : m_poArray(poArray), m_poObjects(poObjects)
    {
    }

    virtual int GetLength() override;
    virtual GDALPDFObject *Get(int nIndex) override;
};

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFStreamPodofo                         */
/* ==================================================================== */
/************************************************************************/

class GDALPDFStreamPodofo : public GDALPDFStream
{
  private:
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    const PoDoFo::PdfObjectStream *m_pStream;
#else
    const PoDoFo::PdfStream *m_pStream;
#endif

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFStreamPodofo)

  public:
    GDALPDFStreamPodofo(
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
        const PoDoFo::PdfObjectStream *
#else
        const PoDoFo::PdfStream *
#endif
            pStream)
        : m_pStream(pStream)
    {
    }

    virtual ~GDALPDFStreamPodofo()
    {
    }

    virtual int64_t GetLength(int64_t nMaxSize = 0) override;
    virtual char *GetBytes() override;

    virtual int64_t GetRawLength() override;
    virtual char *GetRawBytes() override;
};

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFObjectPodofo                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                          GDALPDFObjectPodofo()                       */
/************************************************************************/

GDALPDFObjectPodofo::GDALPDFObjectPodofo(const PoDoFo::PdfObject *po,
                                         const PoDoFo::PdfVecObjects &poObjects)
    : m_po(po), m_poObjects(poObjects)
{
    try
    {
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
        if (m_po->GetDataType() == PoDoFo::PdfDataType::Reference)
        {
            PoDoFo::PdfObject *poObj =
                m_poObjects.GetObject(m_po->GetReference());
            if (poObj)
                m_po = poObj;
        }
#else
        if (m_po->GetDataType() == PoDoFo::ePdfDataType_Reference)
        {
            PoDoFo::PdfObject *poObj =
                m_poObjects.GetObject(m_po->GetReference());
            if (poObj)
                m_po = poObj;
        }
#endif
    }
    catch (PoDoFo::PdfError &oError)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "Invalid PDF : %s",
                 oError.what());
    }
}

/************************************************************************/
/*                         ~GDALPDFObjectPodofo()                       */
/************************************************************************/

GDALPDFObjectPodofo::~GDALPDFObjectPodofo()
{
    delete m_poDict;
    delete m_poArray;
    delete m_poStream;
}

/************************************************************************/
/*                               GetType()                              */
/************************************************************************/

GDALPDFObjectType GDALPDFObjectPodofo::GetType()
{
    try
    {
        switch (m_po->GetDataType())
        {
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
            case PoDoFo::PdfDataType::Null:
                return PDFObjectType_Null;
            case PoDoFo::PdfDataType::Bool:
                return PDFObjectType_Bool;
            case PoDoFo::PdfDataType::Number:
                return PDFObjectType_Int;
            case PoDoFo::PdfDataType::Real:
                return PDFObjectType_Real;
            case PoDoFo::PdfDataType::String:
                return PDFObjectType_String;
            case PoDoFo::PdfDataType::Name:
                return PDFObjectType_Name;
            case PoDoFo::PdfDataType::Array:
                return PDFObjectType_Array;
            case PoDoFo::PdfDataType::Dictionary:
                return PDFObjectType_Dictionary;
            default:
                return PDFObjectType_Unknown;
#else
            case PoDoFo::ePdfDataType_Null:
                return PDFObjectType_Null;
            case PoDoFo::ePdfDataType_Bool:
                return PDFObjectType_Bool;
            case PoDoFo::ePdfDataType_Number:
                return PDFObjectType_Int;
            case PoDoFo::ePdfDataType_Real:
                return PDFObjectType_Real;
            case PoDoFo::ePdfDataType_HexString:
                return PDFObjectType_String;
            case PoDoFo::ePdfDataType_String:
                return PDFObjectType_String;
            case PoDoFo::ePdfDataType_Name:
                return PDFObjectType_Name;
            case PoDoFo::ePdfDataType_Array:
                return PDFObjectType_Array;
            case PoDoFo::ePdfDataType_Dictionary:
                return PDFObjectType_Dictionary;
            default:
                return PDFObjectType_Unknown;
#endif
        }
    }
    catch (PoDoFo::PdfError &oError)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "Invalid PDF : %s",
                 oError.what());
        return PDFObjectType_Unknown;
    }
}

/************************************************************************/
/*                          GetTypeNameNative()                         */
/************************************************************************/

const char *GDALPDFObjectPodofo::GetTypeNameNative()
{
    try
    {
        return m_po->GetDataTypeString();
    }
    catch (PoDoFo::PdfError &oError)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "Invalid PDF : %s",
                 oError.what());
        return "unknown";
    }
}

/************************************************************************/
/*                              GetBool()                               */
/************************************************************************/

int GDALPDFObjectPodofo::GetBool()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    if (m_po->GetDataType() == PoDoFo::PdfDataType::Bool)
#else
    if (m_po->GetDataType() == PoDoFo::ePdfDataType_Bool)
#endif
        return m_po->GetBool();
    else
        return 0;
}

/************************************************************************/
/*                              GetInt()                                */
/************************************************************************/

int GDALPDFObjectPodofo::GetInt()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    if (m_po->GetDataType() == PoDoFo::PdfDataType::Number)
#else
    if (m_po->GetDataType() == PoDoFo::ePdfDataType_Number)
#endif
        return static_cast<int>(m_po->GetNumber());
    else
        return 0;
}

/************************************************************************/
/*                              GetReal()                               */
/************************************************************************/

double GDALPDFObjectPodofo::GetReal()
{
    if (GetType() == PDFObjectType_Real)
        return m_po->GetReal();
    else
        return 0.0;
}

/************************************************************************/
/*                              GetString()                             */
/************************************************************************/

const std::string &GDALPDFObjectPodofo::GetString()
{
    if (GetType() == PDFObjectType_String)
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
        return (osStr = m_po->GetString().GetString());
#else
        return (osStr = m_po->GetString().GetStringUtf8());
#endif
    else
        return (osStr = "");
}

/************************************************************************/
/*                              GetName()                               */
/************************************************************************/

const std::string &GDALPDFObjectPodofo::GetName()
{
    if (GetType() == PDFObjectType_Name)
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
        return (osStr = m_po->GetName().GetString());
#else
        return (osStr = m_po->GetName().GetName());
#endif
    else
        return (osStr = "");
}

/************************************************************************/
/*                             GetDictionary()                          */
/************************************************************************/

GDALPDFDictionary *GDALPDFObjectPodofo::GetDictionary()
{
    if (GetType() != PDFObjectType_Dictionary)
        return nullptr;

    if (m_poDict)
        return m_poDict;

    m_poDict = new GDALPDFDictionaryPodofo(&m_po->GetDictionary(), m_poObjects);
    return m_poDict;
}

/************************************************************************/
/*                                GetArray()                            */
/************************************************************************/

GDALPDFArray *GDALPDFObjectPodofo::GetArray()
{
    if (GetType() != PDFObjectType_Array)
        return nullptr;

    if (m_poArray)
        return m_poArray;

    m_poArray = new GDALPDFArrayPodofo(&m_po->GetArray(), m_poObjects);
    return m_poArray;
}

/************************************************************************/
/*                               GetStream()                            */
/************************************************************************/

GDALPDFStream *GDALPDFObjectPodofo::GetStream()
{
    try
    {
        if (!m_po->HasStream())
            return nullptr;
    }
    catch (PoDoFo::PdfError &oError)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "Invalid object : %s",
                 oError.what());
        return nullptr;
    }
    catch (...)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "Invalid object");
        return nullptr;
    }

    if (m_poStream == nullptr)
        m_poStream = new GDALPDFStreamPodofo(m_po->GetStream());
    return m_poStream;
}

/************************************************************************/
/*                               GetRefNum()                            */
/************************************************************************/

GDALPDFObjectNum GDALPDFObjectPodofo::GetRefNum()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    return GDALPDFObjectNum(m_po->GetIndirectReference().ObjectNumber());
#else
    return GDALPDFObjectNum(m_po->Reference().ObjectNumber());
#endif
}

/************************************************************************/
/*                               GetRefGen()                            */
/************************************************************************/

int GDALPDFObjectPodofo::GetRefGen()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    return m_po->GetIndirectReference().GenerationNumber();
#else
    return m_po->Reference().GenerationNumber();
#endif
}

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFDictionaryPodofo                      */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                         ~GDALPDFDictionaryPodofo()                   */
/************************************************************************/

GDALPDFDictionaryPodofo::~GDALPDFDictionaryPodofo()
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.begin();
    std::map<CPLString, GDALPDFObject *>::iterator oEnd = m_map.end();
    for (; oIter != oEnd; ++oIter)
        delete oIter->second;
}

/************************************************************************/
/*                                  Get()                               */
/************************************************************************/

GDALPDFObject *GDALPDFDictionaryPodofo::Get(const char *pszKey)
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.find(pszKey);
    if (oIter != m_map.end())
        return oIter->second;

    const PoDoFo::PdfObject *poVal = m_poDict->GetKey(PoDoFo::PdfName(pszKey));
    if (poVal)
    {
        GDALPDFObjectPodofo *poObj =
            new GDALPDFObjectPodofo(poVal, m_poObjects);
        m_map[pszKey] = poObj;
        return poObj;
    }
    else
    {
        return nullptr;
    }
}

/************************************************************************/
/*                              GetValues()                             */
/************************************************************************/

std::map<CPLString, GDALPDFObject *> &GDALPDFDictionaryPodofo::GetValues()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    for (const auto &oIter : *m_poDict)
    {
        Get(oIter.first.GetString().c_str());
    }
#else
    for (const auto &oIter : m_poDict->GetKeys())
    {
        Get(oIter.first.GetName().c_str());
    }
#endif
    return m_map;
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFArrayPodofo                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                              GetLength()                             */
/************************************************************************/

int GDALPDFArrayPodofo::GetLength()
{
    return static_cast<int>(m_poArray->GetSize());
}

/************************************************************************/
/*                                Get()                                 */
/************************************************************************/

GDALPDFObject *GDALPDFArrayPodofo::Get(int nIndex)
{
    if (nIndex < 0 || nIndex >= GetLength())
        return nullptr;

    if (m_v.empty())
        m_v.resize(GetLength());

    if (m_v[nIndex] != nullptr)
        return m_v[nIndex].get();

    const PoDoFo::PdfObject &oVal = (*m_poArray)[nIndex];
    m_v[nIndex] = std::make_unique<GDALPDFObjectPodofo>(&oVal, m_poObjects);
    return m_v[nIndex].get();
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFStreamPodofo                        */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                              GetLength()                             */
/************************************************************************/

int64_t GDALPDFStreamPodofo::GetLength(int64_t /* nMaxSize */)
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    PoDoFo::charbuff str;
    try
    {
        m_pStream->CopyToSafe(str);
    }
    catch (PoDoFo::PdfError &e)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "CopyToSafe() failed: %s",
                 e.what());
        return 0;
    }
    return static_cast<int64_t>(str.size());
#else
    char *pBuffer = nullptr;
    PoDoFo::pdf_long nLen = 0;
    try
    {
        m_pStream->GetFilteredCopy(&pBuffer, &nLen);
        PoDoFo::podofo_free(pBuffer);
        return static_cast<int64_t>(nLen);
    }
    catch (PoDoFo::PdfError &e)
    {
    }
    return 0;
#endif
}

/************************************************************************/
/*                               GetBytes()                             */
/************************************************************************/

char *GDALPDFStreamPodofo::GetBytes()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    PoDoFo::charbuff str;
    try
    {
        m_pStream->CopyToSafe(str);
    }
    catch (PoDoFo::PdfError &e)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "CopyToSafe() failed: %s",
                 e.what());
        return nullptr;
    }
    char *pszContent = static_cast<char *>(VSI_MALLOC_VERBOSE(str.size() + 1));
    if (!pszContent)
    {
        return nullptr;
    }
    memcpy(pszContent, str.data(), str.size());
    pszContent[str.size()] = '\0';
    return pszContent;
#else
    char *pBuffer = nullptr;
    PoDoFo::pdf_long nLen = 0;
    try
    {
        m_pStream->GetFilteredCopy(&pBuffer, &nLen);
    }
    catch (PoDoFo::PdfError &e)
    {
        return nullptr;
    }
    char *pszContent = static_cast<char *>(VSI_MALLOC_VERBOSE(nLen + 1));
    if (!pszContent)
    {
        PoDoFo::podofo_free(pBuffer);
        return nullptr;
    }
    memcpy(pszContent, pBuffer, nLen);
    PoDoFo::podofo_free(pBuffer);
    pszContent[nLen] = '\0';
    return pszContent;
#endif
}

/************************************************************************/
/*                             GetRawLength()                           */
/************************************************************************/

int64_t GDALPDFStreamPodofo::GetRawLength()
{
    try
    {
        auto nLen = m_pStream->GetLength();
        return static_cast<int64_t>(nLen);
    }
    catch (PoDoFo::PdfError &e)
    {
    }
    return 0;
}

/************************************************************************/
/*                              GetRawBytes()                           */
/************************************************************************/

char *GDALPDFStreamPodofo::GetRawBytes()
{
#if PODOFO_VERSION_MAJOR > 0 ||                                                \
    (PODOFO_VERSION_MAJOR == 0 && PODOFO_VERSION_MINOR >= 10)
    PoDoFo::charbuff str;
    try
    {
        PoDoFo::StringStreamDevice stream(str);
#ifdef USE_HACK_BECAUSE_PdfInputStream_constructor_is_not_exported_in_podofo_0_11
        auto *poNonConstStream =
            const_cast<PoDoFo::PdfObjectStream *>(m_pStream);
        auto inputStream = poNonConstStream->GetProvider().GetInputStream(
            poNonConstStream->GetParent());
        inputStream->CopyTo(stream);
#else
        // Should work but fails to link because PdfInputStream destructor
        // is not exported
        auto inputStream = m_pStream->GetInputStream(/*raw=*/true);
        inputStream.CopyTo(stream);
#endif
        stream.Flush();
    }
    catch (PoDoFo::PdfError &e)
    {
        CPLError(CE_Failure, CPLE_AppDefined, "CopyToSafe() failed: %s",
                 e.what());
        return nullptr;
    }
    char *pszContent = static_cast<char *>(VSI_MALLOC_VERBOSE(str.size() + 1));
    if (!pszContent)
    {
        return nullptr;
    }
    memcpy(pszContent, str.data(), str.size());
    pszContent[str.size()] = '\0';
    return pszContent;
#else
    char *pBuffer = nullptr;
    PoDoFo::pdf_long nLen = 0;
    try
    {
        m_pStream->GetCopy(&pBuffer, &nLen);
    }
    catch (PoDoFo::PdfError &e)
    {
        return nullptr;
    }
    char *pszContent = static_cast<char *>(VSI_MALLOC_VERBOSE(nLen + 1));
    if (!pszContent)
    {
        PoDoFo::podofo_free(pBuffer);
        return nullptr;
    }
    memcpy(pszContent, pBuffer, nLen);
    PoDoFo::podofo_free(pBuffer);
    pszContent[nLen] = '\0';
    return pszContent;
#endif
}

#endif  // HAVE_PODOFO

#ifdef HAVE_PDFIUM

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFDictionaryPdfium                      */
/* ==================================================================== */
/************************************************************************/

class GDALPDFDictionaryPdfium : public GDALPDFDictionary
{
  private:
    RetainPtr<const CPDF_Dictionary> m_poDict;
    std::map<CPLString, GDALPDFObject *> m_map{};

  public:
    GDALPDFDictionaryPdfium(RetainPtr<const CPDF_Dictionary> poDict)
        : m_poDict(std::move(poDict))
    {
    }

    virtual ~GDALPDFDictionaryPdfium();

    virtual GDALPDFObject *Get(const char *pszKey) override;
    virtual std::map<CPLString, GDALPDFObject *> &GetValues() override;
};

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFArrayPdfium                         */
/* ==================================================================== */
/************************************************************************/

class GDALPDFArrayPdfium : public GDALPDFArray
{
  private:
    const CPDF_Array *m_poArray;
    std::vector<std::unique_ptr<GDALPDFObject>> m_v{};

    CPL_DISALLOW_COPY_ASSIGN(GDALPDFArrayPdfium)

  public:
    GDALPDFArrayPdfium(const CPDF_Array *poArray) : m_poArray(poArray)
    {
    }

    virtual int GetLength() override;
    virtual GDALPDFObject *Get(int nIndex) override;
};

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFStreamPdfium                         */
/* ==================================================================== */
/************************************************************************/

class GDALPDFStreamPdfium : public GDALPDFStream
{
  private:
    RetainPtr<const CPDF_Stream> m_pStream;
    int64_t m_nSize = 0;
    std::unique_ptr<uint8_t, VSIFreeReleaser> m_pData = nullptr;
    int64_t m_nRawSize = 0;
    std::unique_ptr<uint8_t, VSIFreeReleaser> m_pRawData = nullptr;

    void Decompress();
    void FillRaw();

  public:
    GDALPDFStreamPdfium(RetainPtr<const CPDF_Stream> pStream)
        : m_pStream(std::move(pStream))
    {
    }

    virtual int64_t GetLength(int64_t nMaxSize = 0) override;
    virtual char *GetBytes() override;

    virtual int64_t GetRawLength() override;
    virtual char *GetRawBytes() override;
};

/************************************************************************/
/* ==================================================================== */
/*                          GDALPDFObjectPdfium                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                          GDALPDFObjectPdfium()                       */
/************************************************************************/

GDALPDFObjectPdfium::GDALPDFObjectPdfium(RetainPtr<const CPDF_Object> obj)
    : m_obj(std::move(obj))
{
    CPLAssert(m_obj != nullptr);
}

/************************************************************************/
/*                         ~GDALPDFObjectPdfium()                       */
/************************************************************************/

GDALPDFObjectPdfium::~GDALPDFObjectPdfium()
{
    delete m_poDict;
    delete m_poArray;
    delete m_poStream;
}

/************************************************************************/
/*                               Build()                                */
/************************************************************************/

GDALPDFObjectPdfium *
GDALPDFObjectPdfium::Build(RetainPtr<const CPDF_Object> obj)
{
    if (obj == nullptr)
        return nullptr;
    if (obj->GetType() == CPDF_Object::Type::kReference)
    {
        obj = obj->GetDirect();
        if (obj == nullptr)
        {
            CPLError(CE_Failure, CPLE_AppDefined,
                     "Cannot resolve indirect object");
            return nullptr;
        }
    }
    return new GDALPDFObjectPdfium(std::move(obj));
}

/************************************************************************/
/*                               GetType()                              */
/************************************************************************/

GDALPDFObjectType GDALPDFObjectPdfium::GetType()
{
    switch (m_obj->GetType())
    {
        case CPDF_Object::Type::kNullobj:
            return PDFObjectType_Null;
        case CPDF_Object::Type::kBoolean:
            return PDFObjectType_Bool;
        case CPDF_Object::Type::kNumber:
            return (cpl::down_cast<const CPDF_Number *>(m_obj.Get()))
                           ->IsInteger()
                       ? PDFObjectType_Int
                       : PDFObjectType_Real;
        case CPDF_Object::Type::kString:
            return PDFObjectType_String;
        case CPDF_Object::Type::kName:
            return PDFObjectType_Name;
        case CPDF_Object::Type::kArray:
            return PDFObjectType_Array;
        case CPDF_Object::Type::kDictionary:
            return PDFObjectType_Dictionary;
        case CPDF_Object::Type::kStream:
            return PDFObjectType_Dictionary;
        case CPDF_Object::Type::kReference:
            // unresolved reference
            return PDFObjectType_Unknown;
        default:
            CPLAssert(false);
            return PDFObjectType_Unknown;
    }
}

/************************************************************************/
/*                          GetTypeNameNative()                         */
/************************************************************************/

const char *GDALPDFObjectPdfium::GetTypeNameNative()
{
    if (m_obj->GetType() == CPDF_Object::Type::kStream)
        return "stream";
    else
        return "";
}

/************************************************************************/
/*                              GetBool()                               */
/************************************************************************/

int GDALPDFObjectPdfium::GetBool()
{
    return m_obj->GetInteger();
}

/************************************************************************/
/*                              GetInt()                                */
/************************************************************************/

int GDALPDFObjectPdfium::GetInt()
{
    return m_obj->GetInteger();
}

/************************************************************************/
/*                       CPLRoundToMoreLikelyDouble()                   */
/************************************************************************/

// We try to compensate for rounding errors when converting the number
// in the PDF expressed as a string (e.g 297.84) to float32 by pdfium :
// 297.8399963378906 Which is technically correct per the PDF spec, but in
// practice poppler or podofo use double and Geospatial PDF are often encoded
// with double precision.

static double CPLRoundToMoreLikelyDouble(float f)
{
    if (std::round(f) == f)
        return f;

    char szBuffer[80];
    CPLsnprintf(szBuffer, 80, "%f\n", f);
    double d = f;
    char *pszDot = strchr(szBuffer, '.');
    if (pszDot == nullptr)
        return d;
    pszDot++;
    if (pszDot[0] == 0 || pszDot[1] == 0)
        return d;
    if (STARTS_WITH(pszDot + 2, "99"))
    {
        pszDot[2] = 0;
        double d2 = CPLAtof(szBuffer) + 0.01;
        float f2 = static_cast<float>(d2);
        if (f == f2 || nextafterf(f, f + 1.0f) == f2 ||
            nextafterf(f, f - 1.0f) == f2)
            d = d2;
    }
    else if (STARTS_WITH(pszDot + 2, "00"))
    {
        pszDot[2] = 0;
        double d2 = CPLAtof(szBuffer);
        float f2 = static_cast<float>(d2);
        if (f == f2 || nextafterf(f, f + 1.0f) == f2 ||
            nextafterf(f, f - 1.0f) == f2)
            d = d2;
    }
    return d;
}

/************************************************************************/
/*                              GetReal()                               */
/************************************************************************/

double GDALPDFObjectPdfium::GetReal()
{
    return CPLRoundToMoreLikelyDouble(m_obj->GetNumber());
}

/************************************************************************/
/*                              GetString()                             */
/************************************************************************/

const std::string &GDALPDFObjectPdfium::GetString()
{
    if (GetType() == PDFObjectType_String)
    {
        const auto bs = m_obj->GetString();
        // If empty string, code crashes
        if (bs.IsEmpty())
            return (osStr = "");
        return (osStr = GDALPDFGetUTF8StringFromBytes(
                    reinterpret_cast<const GByte *>(bs.c_str()),
                    static_cast<int>(bs.GetLength())));
    }
    else
        return (osStr = "");
}

/************************************************************************/
/*                              GetName()                               */
/************************************************************************/

const std::string &GDALPDFObjectPdfium::GetName()
{
    if (GetType() == PDFObjectType_Name)
        return (osStr = m_obj->GetString().c_str());
    else
        return (osStr = "");
}

/************************************************************************/
/*                             GetDictionary()                          */
/************************************************************************/

GDALPDFDictionary *GDALPDFObjectPdfium::GetDictionary()
{
    if (GetType() != PDFObjectType_Dictionary)
        return nullptr;

    if (m_poDict)
        return m_poDict;

    m_poDict = new GDALPDFDictionaryPdfium(m_obj->GetDict());
    return m_poDict;
}

/************************************************************************/
/*                                GetArray()                            */
/************************************************************************/

GDALPDFArray *GDALPDFObjectPdfium::GetArray()
{
    if (GetType() != PDFObjectType_Array)
        return nullptr;

    if (m_poArray)
        return m_poArray;

    m_poArray =
        new GDALPDFArrayPdfium(cpl::down_cast<const CPDF_Array *>(m_obj.Get()));
    return m_poArray;
}

/************************************************************************/
/*                               GetStream()                            */
/************************************************************************/

GDALPDFStream *GDALPDFObjectPdfium::GetStream()
{
    if (m_obj->GetType() != CPDF_Object::Type::kStream)
        return nullptr;

    if (m_poStream)
        return m_poStream;
    auto pStream = pdfium::WrapRetain(m_obj->AsStream());
    if (pStream)
    {
        m_poStream = new GDALPDFStreamPdfium(std::move(pStream));
        return m_poStream;
    }
    else
        return nullptr;
}

/************************************************************************/
/*                               GetRefNum()                            */
/************************************************************************/

GDALPDFObjectNum GDALPDFObjectPdfium::GetRefNum()
{
    return GDALPDFObjectNum(m_obj->GetObjNum());
}

/************************************************************************/
/*                               GetRefGen()                            */
/************************************************************************/

int GDALPDFObjectPdfium::GetRefGen()
{
    return m_obj->GetGenNum();
}

/************************************************************************/
/* ==================================================================== */
/*                         GDALPDFDictionaryPdfium                      */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                         ~GDALPDFDictionaryPdfium()                   */
/************************************************************************/

GDALPDFDictionaryPdfium::~GDALPDFDictionaryPdfium()
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.begin();
    std::map<CPLString, GDALPDFObject *>::iterator oEnd = m_map.end();
    for (; oIter != oEnd; ++oIter)
        delete oIter->second;
}

/************************************************************************/
/*                                  Get()                               */
/************************************************************************/

GDALPDFObject *GDALPDFDictionaryPdfium::Get(const char *pszKey)
{
    std::map<CPLString, GDALPDFObject *>::iterator oIter = m_map.find(pszKey);
    if (oIter != m_map.end())
        return oIter->second;

    ByteString pdfiumKey(pszKey);
    GDALPDFObjectPdfium *poObj =
        GDALPDFObjectPdfium::Build(m_poDict->GetObjectFor(pdfiumKey));
    if (poObj)
    {
        m_map[pszKey] = poObj;
        return poObj;
    }
    else
    {
        return nullptr;
    }
}

/************************************************************************/
/*                              GetValues()                             */
/************************************************************************/

std::map<CPLString, GDALPDFObject *> &GDALPDFDictionaryPdfium::GetValues()
{
    CPDF_DictionaryLocker dictIterator(m_poDict);
    for (const auto &iter : dictIterator)
    {
        // No object for this key
        if (!iter.second)
            continue;

        const char *pszKey = iter.first.c_str();
        // Objects exists in the map
        if (m_map.find(pszKey) != m_map.end())
            continue;
        GDALPDFObjectPdfium *poObj = GDALPDFObjectPdfium::Build(iter.second);
        if (poObj == nullptr)
            continue;
        m_map[pszKey] = poObj;
    }

    return m_map;
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFArrayPdfium                         */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                              GetLength()                             */
/************************************************************************/

int GDALPDFArrayPdfium::GetLength()
{
    return static_cast<int>(m_poArray->size());
}

/************************************************************************/
/*                                Get()                                 */
/************************************************************************/

GDALPDFObject *GDALPDFArrayPdfium::Get(int nIndex)
{
    if (nIndex < 0 || nIndex >= GetLength())
        return nullptr;

    if (m_v.empty())
        m_v.resize(GetLength());

    if (m_v[nIndex] != nullptr)
        return m_v[nIndex].get();

    auto poObj = std::unique_ptr<GDALPDFObjectPdfium>(
        GDALPDFObjectPdfium::Build(m_poArray->GetObjectAt(nIndex)));
    if (poObj == nullptr)
        return nullptr;
    m_v[nIndex] = std::move(poObj);
    return m_v[nIndex].get();
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALPDFStreamPdfium                        */
/* ==================================================================== */
/************************************************************************/

void GDALPDFStreamPdfium::Decompress()
{
    if (m_pData != nullptr)
        return;
    auto acc(pdfium::MakeRetain<CPDF_StreamAcc>(m_pStream));
    acc->LoadAllDataFiltered();
    m_nSize = static_cast<int64_t>(acc->GetSize());
    m_pData.reset();
    const auto nSize = static_cast<size_t>(m_nSize);
    if (static_cast<int64_t>(nSize) != m_nSize)
    {
        m_nSize = 0;
    }
    if (m_nSize)
    {
        m_pData.reset(static_cast<uint8_t *>(VSI_MALLOC_VERBOSE(nSize)));
        if (!m_pData)
            m_nSize = 0;
        else
            memcpy(&m_pData.get()[0], acc->DetachData().data(), nSize);
    }
}

/************************************************************************/
/*                              GetLength()                             */
/************************************************************************/

int64_t GDALPDFStreamPdfium::GetLength(int64_t /* nMaxSize */)
{
    Decompress();
    return m_nSize;
}

/************************************************************************/
/*                               GetBytes()                             */
/************************************************************************/

char *GDALPDFStreamPdfium::GetBytes()
{
    size_t nLength = static_cast<size_t>(GetLength());
    if (nLength == 0)
        return nullptr;
    char *pszContent = static_cast<char *>(VSI_MALLOC_VERBOSE(nLength + 1));
    if (!pszContent)
        return nullptr;
    memcpy(pszContent, m_pData.get(), nLength);
    pszContent[nLength] = '\0';
    return pszContent;
}

/************************************************************************/
/*                                FillRaw()                             */
/************************************************************************/

void GDALPDFStreamPdfium::FillRaw()
{
    if (m_pRawData != nullptr)
        return;
    auto acc(pdfium::MakeRetain<CPDF_StreamAcc>(m_pStream));
    acc->LoadAllDataRaw();
    m_nRawSize = static_cast<int64_t>(acc->GetSize());
    m_pRawData.reset();
    const auto nSize = static_cast<size_t>(m_nRawSize);
    if (static_cast<int64_t>(nSize) != m_nRawSize)
    {
        m_nRawSize = 0;
    }
    if (m_nRawSize)
    {
        m_pRawData.reset(
            static_cast<uint8_t *>(VSI_MALLOC_VERBOSE(m_nRawSize)));
        if (!m_pRawData)
            m_nRawSize = 0;
        else
            memcpy(&m_pRawData.get()[0], acc->DetachData().data(), m_nRawSize);
    }
}

/************************************************************************/
/*                            GetRawLength()                            */
/************************************************************************/

int64_t GDALPDFStreamPdfium::GetRawLength()
{
    FillRaw();
    return m_nRawSize;
}

/************************************************************************/
/*                             GetRawBytes()                            */
/************************************************************************/

char *GDALPDFStreamPdfium::GetRawBytes()
{
    size_t nLength = static_cast<size_t>(GetRawLength());
    if (nLength == 0)
        return nullptr;
    char *pszContent =
        static_cast<char *>(VSI_MALLOC_VERBOSE(sizeof(char) * (nLength + 1)));
    if (!pszContent)
        return nullptr;
    memcpy(pszContent, m_pRawData.get(), nLength);
    pszContent[nLength] = '\0';
    return pszContent;
}

#endif  // HAVE_PDFIUM
