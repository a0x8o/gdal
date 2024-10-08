/******************************************************************************
 *
 * Project:  WCS Client Driver
 * Purpose:  Implementation of utilities.
 * Author:   Ari Jolma <ari dot jolma at gmail dot com>
 *
 ******************************************************************************
 * Copyright (c) 2006, Frank Warmerdam
 * Copyright (c) 2008-2013, Even Rouault <even dot rouault at spatialys.com>
 * Copyright (c) 2017, Ari Jolma
 * Copyright (c) 2017, Finnish Environment Institute
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#ifndef WCSUTILS_H_INCLUDED
#define WCSUTILS_H_INCLUDED

#include "cpl_string.h"
#include "cpl_minixml.h"
#include <vector>

namespace WCSUtils
{

void Swap(double &a, double &b);

std::string URLEncode(const std::string &str);

std::string URLRemoveKey(const char *url, const std::string &key);

std::vector<std::string> &SwapFirstTwo(std::vector<std::string> &array);

std::vector<std::string> Split(const char *value, const char *delim,
                               bool swap_the_first_two = false);

std::string Join(const std::vector<std::string> &array, const char *delim,
                 bool swap_the_first_two = false);

std::vector<int> Ilist(const std::vector<std::string> &array,
                       unsigned int from = 0, size_t count = std::string::npos);

std::vector<double> Flist(const std::vector<std::string> &array,
                          unsigned int from = 0,
                          size_t count = std::string::npos);

// index of string or integer in an array
// indexes of strings in an array
// index of key in a key value pairs
int IndexOf(const std::string &str, const std::vector<std::string> &array);
int IndexOf(int i, const std::vector<int> &array);
std::vector<int> IndexOf(const std::vector<std::string> &strs,
                         const std::vector<std::string> &array);
int IndexOf(const std::string &key,
            const std::vector<std::vector<std::string>> &kvps);

bool Contains(const std::vector<int> &array, int value);

std::string FromParenthesis(const std::string &s);

std::vector<std::string>
ParseSubset(const std::vector<std::string> &subset_array,
            const std::string &dim);

bool FileIsReadable(const std::string &filename);

std::string RemoveExt(const std::string &filename);

bool MakeDir(const std::string &dirname);

CPLXMLNode *SearchChildWithValue(CPLXMLNode *node, const char *path,
                                 const char *value);

bool CPLGetXMLBoolean(CPLXMLNode *poRoot, const char *pszPath);

bool CPLUpdateXML(CPLXMLNode *poRoot, const char *pszPath,
                  const char *new_value);

void XMLCopyMetadata(CPLXMLNode *node, CPLXMLNode *metadata,
                     const std::string &key);

bool SetupCache(std::string &cache, bool clear);

std::vector<std::string> ReadCache(const std::string &cache);

bool DeleteEntryFromCache(const std::string &cache, const std::string &key,
                          const std::string &value);

CPLErr SearchCache(const std::string &cache, const std::string &url,
                   std::string &filename, const std::string &ext, bool &found);

CPLErr AddEntryToCache(const std::string &cache, const std::string &url,
                       std::string &filename, const std::string &ext);

CPLXMLNode *AddSimpleMetaData(char ***metadata, CPLXMLNode *node,
                              std::string &path, const std::string &from,
                              const std::vector<std::string> &keys);

std::string GetKeywords(CPLXMLNode *root, const std::string &path,
                        const std::string &kw);

std::string ParseCRS(CPLXMLNode *node);

bool CRS2Projection(const std::string &crs, OGRSpatialReference *sr,
                    char **projection);

bool CRSImpliesAxisOrderSwap(const std::string &crs, bool &swap,
                             char **projection = nullptr);

std::vector<std::vector<int>>
ParseGridEnvelope(CPLXMLNode *node, bool swap_the_first_two = false);

std::vector<std::string> ParseBoundingBox(CPLXMLNode *node);

}  // namespace WCSUtils

#endif /* WCSUTILS_H_INCLUDED */
