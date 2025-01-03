#!/usr/bin/env pytest
# -*- coding: utf-8 -*-
###############################################################################
#
# Project:  GDAL/OGR Test Suite
# Purpose:  Test IRIS driver
# Author:   Even Rouault, <even dot rouault at spatialys.com>
#
###############################################################################
# Copyright (c) 2012-2013, Even Rouault <even dot rouault at spatialys.com>
#
# SPDX-License-Identifier: MIT
###############################################################################

import gdaltest
import pytest

from osgeo import gdal

pytestmark = pytest.mark.require_driver("IRIS")

###############################################################################
# Test reading a - fake - IRIS dataset


def test_iris_1():

    tst = gdaltest.GDALTest("IRIS", "iris/fakeiris.dat", 1, 65532)
    tst.testOpen()


###############################################################################
# Test reading a real world IRIS dataset.


def test_iris_2():

    ds = gdal.Open("data/iris/iristest.dat")
    assert ds.GetRasterBand(1).Checksum() == 52872

    ds.GetProjectionRef()
    # expected_wkt = """PROJCS["unnamed",GEOGCS["unnamed ellipse",DATUM["unknown",SPHEROID["unnamed",6371000.5,0]],PRIMEM["Greenwich",0],UNIT["degree",0.0174532925199433]],PROJECTION["Mercator_1SP"],PARAMETER["central_meridian",0],PARAMETER["scale_factor",1],PARAMETER["false_easting",0],PARAMETER["false_northing",0]]"""
    # got_srs = osr.SpatialReference(got_wkt)
    # expected_srs = osr.SpatialReference(expected_wkt)

    # There are some differences in the values of the parameters between Linux
    # and Windows not sure if it is only due to rounding differences,
    # different proj versions, etc...
    # if got_srs.IsSame(expected_srs) != 1:
    #    gdaltest.post_reason('fail')
    #    print('')
    #    print(expected_wkt)
    #    print(got_wkt)
    #    return 'fail'

    got_gt = ds.GetGeoTransform()
    expected_gt = [
        16435.721785269096,
        1370.4263720754534,
        0.0,
        5289830.4584420761,
        0.0,
        -1357.6498705837876,
    ]
    for i in range(6):
        assert not (
            (expected_gt[i] == 0.0 and got_gt[i] != 0.0)
            or (
                expected_gt[i] != 0.0
                and abs(got_gt[i] - expected_gt[i]) / abs(expected_gt[i]) > 1e-5
            )
        )

    expected_metadata = [
        "AZIMUTH_SMOOTHING_FOR_SHEAR=0.0",
        "CAPPI_BOTTOM_HEIGHT=1000.0 m",
        "COMPOSITED_PRODUCT=YES",
        "COMPOSITED_PRODUCT_MASK=0x0000080c",
        "DATA_TYPE=Clutter Corrected H reflectivity (1 byte)",
        "DATA_TYPE_CODE=dBZ",
        "DATA_TYPE_INPUT=Clutter Corrected H reflectivity (1 byte)",
        "DATA_TYPE_INPUT_CODE=dBZ",
        "DATA_TYPE_UNITS=dBZ",
        "GROUND_HEIGHT=523 m",
        "INGEST_HARDWARE_NAME=composada       ",
        "INGEST_SITE_IRIS_VERSION=8.12",
        "INGEST_SITE_NAME=composada       ",
        "MAX_AGE_FOR_SHEAR_VVP_CORRECTION=600 s",
        "NYQUIST_VELOCITY=6.00 m/s",
        "PRF=450 Hz",
        "PRODUCT=CAPPI",
        "PRODUCT_CONFIGURATION_NAME=CAPPI250CAT ",
        "PRODUCT_ID=3",
        "PRODUCT_SITE_IRIS_VERSION=8.12",
        "PRODUCT_SITE_NAME=SMCXRADSRV01    ",
        "RADAR_HEIGHT=542 m",
        "TASK_NAME=PPIVOL_A    ",
        "TIME_INPUT_INGEST_SWEEP=2012-04-19 14:48:05",
        "TIME_PRODUCT_GENERATED=2012-04-19 14:48:30",
        "WAVELENGTH=5.33 cm",
    ]
    got_metadata = ds.GetMetadata()

    for md in expected_metadata:
        key = md[0 : md.find("=")]
        value = md[md.find("=") + 1 :]
        assert got_metadata[key] == value, "did not find %s" % key
