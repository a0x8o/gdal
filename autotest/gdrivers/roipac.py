#!/usr/bin/env pytest
# -*- coding: utf-8 -*-
###############################################################################
#
# Project:  GDAL/OGR Test Suite
# Purpose:  Test ROI_PAC format driver.
# Author:   Matthieu Volat <matthieu.volat@ujf-grenoble.fr>
#
###############################################################################
# Copyright (c) 2014, Matthieu Volat <matthieu.volat@ujf-grenoble.fr>
#
# SPDX-License-Identifier: MIT
###############################################################################

import gdaltest
import pytest

from osgeo import gdal

pytestmark = pytest.mark.require_driver("ROI_PAC")

###############################################################################
# Perform simple read test.


def test_roipac_1():

    tst = gdaltest.GDALTest("roi_pac", "roipac/srtm.dem", 1, 64074)

    prj = """GEOGCS["WGS 84",
    DATUM["WGS_1984",
        SPHEROID["WGS 84",6378137,298.257223563,
            AUTHORITY["EPSG","7030"]],
        TOWGS84[0,0,0,0,0,0,0],
        AUTHORITY["EPSG","6326"]],
    PRIMEM["Greenwich",0,
        AUTHORITY["EPSG","8901"]],
    UNIT["degree",0.0174532925199433,
        AUTHORITY["EPSG","9108"]],
    AUTHORITY["EPSG","4326"]]"""

    tst.testOpen(
        check_prj=prj,
        check_gt=(-180.0083333, 0.0083333333, 0.0, -59.9916667, 0.0, -0.0083333333),
    )


###############################################################################
# Test reading of metadata from the ROI_PAC metadata domain


def test_roipac_2():

    ds = gdal.Open("data/roipac/srtm.dem")
    val = ds.GetMetadataItem("YMAX", "ROI_PAC")
    assert val == "9"


###############################################################################
# Verify this can be exported losslessly.


def test_roipac_3():

    tst = gdaltest.GDALTest("roi_pac", "roipac/srtm.dem", 1, 64074)
    tst.testCreateCopy(check_gt=1, new_filename="strm.tst.dem")


###############################################################################
# Verify VSIF*L capacity


def test_roipac_4():

    tst = gdaltest.GDALTest("roi_pac", "roipac/srtm.dem", 1, 64074)
    tst.testCreateCopy(check_gt=1, new_filename="strm.tst.dem", vsimem=1)


###############################################################################
# Verify offset/scale metadata reading


def test_roipac_5():

    ds = gdal.Open("data/roipac/srtm.dem")
    band = ds.GetRasterBand(1)
    offset = band.GetOffset()
    assert offset == 1
    scale = band.GetScale()
    assert scale == 2


###############################################################################
# Test .flg


def test_roipac_6():

    tst = gdaltest.GDALTest("roi_pac", "byte.tif", 1, 4672)
    with gdal.quiet_errors():
        tst.testCreateCopy(check_gt=1, new_filename="byte.flg", vsimem=1)
