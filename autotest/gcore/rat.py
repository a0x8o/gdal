#!/usr/bin/env pytest
# -*- coding: utf-8 -*-
###############################################################################
#
# Project:  GDAL/OGR Test Suite
# Purpose:  Test RasterAttributeTables services from Python.
# Author:   Frank Warmerdam <warmerdam@pobox.com>
#
###############################################################################
# Copyright (c) 2009, Frank Warmerdam <warmerdam@pobox.com>
# Copyright (c) 2009-2013, Even Rouault <even dot rouault at spatialys.com>
#
# SPDX-License-Identifier: MIT
###############################################################################


import pytest

from osgeo import gdal

###############################################################################
# Create a raster attribute table.


@pytest.fixture()
def test_rat():

    rat = gdal.RasterAttributeTable()

    rat.CreateColumn("Value", gdal.GFT_Integer, gdal.GFU_MinMax)
    rat.CreateColumn("Count", gdal.GFT_Integer, gdal.GFU_PixelCount)

    rat.SetRowCount(3)
    rat.SetValueAsInt(0, 0, 10)
    rat.SetValueAsInt(0, 1, 100)
    rat.SetValueAsInt(1, 0, 11)
    rat.SetValueAsInt(1, 1, 200)
    rat.SetValueAsInt(2, 0, 12)
    rat.SetValueAsInt(2, 1, 90)

    return rat


def test_rat_1(test_rat):

    rat2 = test_rat.Clone()

    assert rat2.GetColumnCount() == 2, "wrong column count"

    assert rat2.GetRowCount() == 3, "wrong row count"

    assert rat2.GetNameOfCol(1) == "Count", "wrong column name"

    assert rat2.GetUsageOfCol(1) == gdal.GFU_PixelCount, "wrong column usage"

    assert rat2.GetTypeOfCol(1) == gdal.GFT_Integer, "wrong column type"

    assert rat2.GetRowOfValue(11.0) == 1, "wrong row for value"

    assert rat2.GetValueAsInt(1, 1) == 200, "wrong field value."


###############################################################################
# Save a RAT in a file, written to .aux.xml, read it back and check it.


@pytest.mark.require_driver("PNM")
def test_rat_2(test_rat):

    ds = gdal.GetDriverByName("PNM").Create("tmp/rat_2.pnm", 100, 90, 1, gdal.GDT_Byte)
    ds.GetRasterBand(1).SetDefaultRAT(test_rat)

    ds = None

    ds = gdal.Open("tmp/rat_2.pnm", gdal.GA_Update)
    rat2 = ds.GetRasterBand(1).GetDefaultRAT()

    assert rat2.GetColumnCount() == 2, "wrong column count"

    assert rat2.GetRowCount() == 3, "wrong row count"

    assert rat2.GetNameOfCol(1) == "Count", "wrong column name"

    assert rat2.GetUsageOfCol(1) == gdal.GFU_PixelCount, "wrong column usage"

    assert rat2.GetTypeOfCol(1) == gdal.GFT_Integer, "wrong column type"

    assert rat2.GetRowOfValue(11.0) == 1, "wrong row for value"

    assert rat2.GetValueAsInt(1, 1) == 200, "wrong field value."

    # unset the RAT
    ds.GetRasterBand(1).SetDefaultRAT(None)

    ds = None

    ds = gdal.Open("tmp/rat_2.pnm")
    rat = ds.GetRasterBand(1).GetDefaultRAT()
    ds = None
    assert rat is None, "expected a NULL RAT."

    gdal.GetDriverByName("PNM").Delete("tmp/rat_2.pnm")


###############################################################################
# Save an empty RAT (#5451)


def test_rat_3():

    ds = gdal.GetDriverByName("GTiff").Create("/vsimem/rat_3.tif", 1, 1)
    ds.GetRasterBand(1).SetDefaultRAT(gdal.RasterAttributeTable())
    ds = None

    gdal.GetDriverByName("GTiff").Delete("/vsimem/rat_3.tif")


###############################################################################
# Edit an existing RAT (#3783)


def test_rat_4():

    # Create test RAT
    ds = gdal.GetDriverByName("GTiff").Create("/vsimem/rat_4.tif", 1, 1)
    rat = gdal.RasterAttributeTable()
    rat.CreateColumn("VALUE", gdal.GFT_Integer, gdal.GFU_MinMax)
    rat.CreateColumn("CLASS", gdal.GFT_String, gdal.GFU_Name)
    rat.SetValueAsInt(0, 0, 111)
    rat.SetValueAsString(0, 1, "Class1")
    ds.GetRasterBand(1).SetDefaultRAT(rat)
    ds = None

    # Verify
    ds = gdal.OpenEx("/vsimem/rat_4.tif")
    gdal_band = ds.GetRasterBand(1)
    rat = gdal_band.GetDefaultRAT()
    assert rat.GetValueAsInt(0, 0) == 111
    ds = None

    # Replace existing RAT
    rat = gdal.RasterAttributeTable()
    rat.CreateColumn("VALUE", gdal.GFT_Integer, gdal.GFU_MinMax)
    rat.CreateColumn("CLASS", gdal.GFT_String, gdal.GFU_Name)
    rat.SetValueAsInt(0, 0, 222)
    rat.SetValueAsString(0, 1, "Class1")
    ds = gdal.OpenEx("/vsimem/rat_4.tif", gdal.OF_RASTER | gdal.OF_UPDATE)
    gdal_band = ds.GetRasterBand(1)
    gdal_band.SetDefaultRAT(rat)
    ds = None

    # Verify
    ds = gdal.OpenEx("/vsimem/rat_4.tif")
    gdal_band = ds.GetRasterBand(1)
    rat = gdal_band.GetDefaultRAT()
    assert rat is not None
    assert rat.GetValueAsInt(0, 0) == 222
    ds = None

    gdal.GetDriverByName("GTiff").Delete("/vsimem/rat_4.tif")


##############################################################################
