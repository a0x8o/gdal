#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ******************************************************************************
#
#  Project:  GDAL
#  Purpose:  module for loading gdal.ColorTable from a file using ColorPalette
#  Author:   Idan Miara <idan@miara.com>
#
# ******************************************************************************
#  Copyright (c) 2020, Idan Miara <idan@miara.com>
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
# ******************************************************************************

import os
import tempfile
from typing import Optional, Union

from osgeo import gdal
from osgeo_utils.auxiliary.base import PathLikeOrStr
from osgeo_utils.auxiliary.util import open_ds, PathOrDS
from osgeo_utils.auxiliary.color_palette import get_color_palette, ColorPaletteOrPathOrStrings, ColorPalette

ColorTableLike = Union[gdal.ColorTable, ColorPaletteOrPathOrStrings]


def get_color_table_from_raster(path_or_ds: PathOrDS) -> Optional[gdal.ColorTable]:
    ds = open_ds(path_or_ds, silent_fail=True)
    if ds is None:
        return None
    ct = ds.GetRasterBand(1).GetRasterColorTable()
    if ct is None:
        return None
    return ct.Clone()


<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
=======
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 9a931cd0f4 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> OSGeo-master:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 34342977ef (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> a853d8a9a9 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
>>>>>>> 1663519ad8 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 2767799a55 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 1663519ad8 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 01a6ad8c2b (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
>>>>>>> ba1e26a07d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 9a931cd0f4 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2d0fae300d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> a853d8a9a9 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2767799a55 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
=======
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
def color_table_from_color_palette(pal: ColorPalette, color_table: gdal.ColorTable,
                                   fill_missing_colors=True, min_key=0, max_key=255) -> bool:
    """ returns None if pal has no values, otherwise returns a gdal.ColorTable from the given ColorPalette"""
    if not pal.pal or not pal.is_numeric():
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
<<<<<<< HEAD:gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
        # palette has no values or not numeric
        return False
=======
=======
>>>>>>> 34342977ef (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> a853d8a9a9 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ba1e26a07d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
        raise Exception('palette has no values or not fully numeric')
=======
def get_color_table(color_palette_or_path_or_strings_or_ds: Optional[ColorTableLike],
                    min_key=0, max_key=255, fill_missing_colors=True) -> Optional[gdal.ColorTable]:
    if (color_palette_or_path_or_strings_or_ds is None or
       isinstance(color_palette_or_path_or_strings_or_ds, gdal.ColorTable)):
        return color_palette_or_path_or_strings_or_ds

    if isinstance(color_palette_or_path_or_strings_or_ds, gdal.Dataset):
        return get_color_table_from_raster(color_palette_or_path_or_strings_or_ds)

    try:
        pal = get_color_palette(color_palette_or_path_or_strings_or_ds)
    except:
        # the input might be a filename of a raster file
        return get_color_table_from_raster(color_palette_or_path_or_strings_or_ds)
    color_table = gdal.ColorTable()
>>>>>>> 5742ec588f (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ba1e26a07d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 2d0fae300d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
        # palette has no values or not numeric
        return False
>>>>>>> 2ac37d0503 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 9a931cd0f4 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> OSGeo-master:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 34342977ef (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> a853d8a9a9 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
=======
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> OSGeo-master:swig/python/gdal-utils/osgeo_utils/auxiliary/color_table.py
=======
>>>>>>> 34342977ef (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 1663519ad8 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2767799a55 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
>>>>>>> 1663519ad8 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> 1663519ad8 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 01a6ad8c2b (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ba1e26a07d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 9a931cd0f4 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 2d0fae300d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> e7a1893785 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> f16d61da56 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
    if fill_missing_colors:
        keys = sorted(list(pal.pal.keys()))
        if min_key is None:
            min_key = keys[0]
        if max_key is None:
            max_key = keys[-1]
        c = pal.color_to_color_entry(pal.pal[keys[0]])
        for key in range(min_key, max_key + 1):
            if key in keys:
                c = pal.color_to_color_entry(pal.pal[key])
            color_table.SetColorEntry(key, c)
    else:
        for key, col in pal.pal.items():
            color_table.SetColorEntry(key, pal.color_to_color_entry(col))  # set color for each key
    return True


def get_color_table(color_palette_or_path_or_strings_or_ds: Optional[ColorTableLike],
                    **kwargs) -> Optional[gdal.ColorTable]:
    if (color_palette_or_path_or_strings_or_ds is None or
       isinstance(color_palette_or_path_or_strings_or_ds, gdal.ColorTable)):
        return color_palette_or_path_or_strings_or_ds

    if isinstance(color_palette_or_path_or_strings_or_ds, gdal.Dataset):
        return get_color_table_from_raster(color_palette_or_path_or_strings_or_ds)

    try:
        pal = get_color_palette(color_palette_or_path_or_strings_or_ds)
        color_table = gdal.ColorTable()
        res = color_table_from_color_palette(pal, color_table, **kwargs)
        return color_table if res else None
    except:
        # the input might be a filename of a raster file
        return get_color_table_from_raster(color_palette_or_path_or_strings_or_ds)


def is_fixed_color_table(color_table: gdal.ColorTable, c=(0, 0, 0, 0)) -> bool:
    for i in range(color_table.GetCount()):
        color_entry: gdal.ColorEntry = color_table.GetColorEntry(i)
        if color_entry != c:
            return False
    return True


def get_fixed_color_table(c=(0, 0, 0, 0), count=1):
    color_table = gdal.ColorTable()
    for i in range(count):
        color_table.SetColorEntry(i, c)
    return color_table


def are_equal_color_table(color_table1: gdal.ColorTable, color_table2: gdal.ColorTable) -> bool:
    if color_table1.GetCount() != color_table2.GetCount():
        return False
    for i in range(color_table1.GetCount()):
        color_entry1: gdal.ColorEntry = color_table1.GetColorEntry(i)
        color_entry2: gdal.ColorEntry = color_table2.GetColorEntry(i)
        if color_entry1 != color_entry2:
            return False
    return True


def write_color_table_to_file(color_table: gdal.ColorTable, color_filename: Optional[PathLikeOrStr]):
    tmp_fd = None
    if color_filename is None:
        tmp_fd, color_filename = tempfile.mkstemp(suffix='.txt')
    os.makedirs(os.path.dirname(color_filename), exist_ok=True)
    with open(color_filename, mode='w') as fp:
        for i in range(color_table.GetCount()):
            color_entry = color_table.GetColorEntry(i)
            color_entry = ' '.join(str(c) for c in color_entry)
            fp.write('{} {}\n'.format(i, color_entry))
    if tmp_fd:
        os.close(tmp_fd)
    return color_filename
