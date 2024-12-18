#!/usr/bin/env pytest
# -*- coding: utf-8 -*-
###############################################################################
#
# Project:  GDAL/OGR Test Suite
# Purpose:  gdal2tiles.py testing
# Author:   Gregory Bataille <gregory.bataille@gmail.com>
#
###############################################################################
# Copyright (c) 2017, Gregory Bataille <gregory.bataille@gmail.com>
#
# SPDX-License-Identifier: MIT
###############################################################################

from unittest import TestCase, mock

import pytest

from osgeo import gdal, osr
from osgeo_utils import gdal2tiles


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)
        self.__dict__ = self


class ReprojectDatasetTest(TestCase):
    def setUp(self):
        self.DEFAULT_OPTIONS = {
            "verbose": True,
            "resampling": "near",
            "title": "",
            "url": "",
        }
        self.DEFAULT_ATTRDICT_OPTIONS = AttrDict(self.DEFAULT_OPTIONS)
        self.mercator_srs = osr.SpatialReference()
        self.mercator_srs.ImportFromEPSG(3857)
        self.geodetic_srs = osr.SpatialReference()
        self.geodetic_srs.SetAxisMappingStrategy(osr.OAMS_TRADITIONAL_GIS_ORDER)
        self.geodetic_srs.ImportFromEPSG(4326)

    def test_raises_if_no_from_or_to_srs(self):
        with self.assertRaises(gdal2tiles.GDALError):
            gdal2tiles.reproject_dataset(None, None, self.mercator_srs)
        with self.assertRaises(gdal2tiles.GDALError):
            gdal2tiles.reproject_dataset(None, self.mercator_srs, None)

    def test_returns_dataset_unchanged_if_in_destination_srs_and_no_gcps(self):
        from_ds = mock.MagicMock()
        from_ds.GetGCPCount = mock.MagicMock(return_value=0)

        to_ds = gdal2tiles.reproject_dataset(
            from_ds, self.mercator_srs, self.mercator_srs
        )

        self.assertEqual(from_ds, to_ds)

    @mock.patch("osgeo_utils.gdal2tiles.gdal", spec=gdal)
    def test_returns_warped_vrt_dataset_when_from_srs_different_from_to_srs(
        self, mock_gdal
    ):
        mock_gdal.AutoCreateWarpedVRT = mock.MagicMock(spec=gdal.Dataset)
        from_ds = mock.MagicMock(spec=gdal.Dataset)
        from_ds.GetGCPCount = mock.MagicMock(return_value=0)

        gdal2tiles.reproject_dataset(from_ds, self.mercator_srs, self.geodetic_srs)

        mock_gdal.AutoCreateWarpedVRT.assert_called_with(
            from_ds, self.mercator_srs.ExportToWkt(), self.geodetic_srs.ExportToWkt()
        )

    def test_4326_to_3857_longitude_beyond_180(self):
        from_ds = gdal.GetDriverByName("MEM").Create("", 10, 10)
        from_ds.SetGeoTransform([-180, 36.01, 0, 90, 0, -18])
        to_ds = gdal2tiles.reproject_dataset(
            from_ds, self.geodetic_srs, self.mercator_srs
        )
        assert to_ds.RasterXSize == 13
        assert to_ds.RasterYSize == 13
        assert to_ds.GetGeoTransform() == pytest.approx(
            (
                -20037508.342789244,
                3082693.591198345,
                0.0,
                20037508.342789248,
                0.0,
                -3082693.5911983456,
            )
        )
