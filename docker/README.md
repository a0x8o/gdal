GDAL Docker images
==================

This directory contains a number of Dockerfile for different configurations.
Each directory contains a `./build.sh` for convenient building of the image.

Note: the mention of the overall licensing terms of the GDAL build is to the
best of our knowledge and not guaranteed. Users should check by themselves.

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
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 19e1deeb18 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> d8608c8f1e (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 8e8bcf6841 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
=======
>>>>>>> 311244091e (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
=======
=======
=======
>>>>>>> d8608c8f1e (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD:gdal/docker/README.md
=======
>>>>>>> 126d56369a (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
<<<<<<< HEAD:docker/README.md
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> 03b2261518 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 139f18cc33 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 19e1deeb18 (Merge branch 'master' of github.com:OSGeo/gdal)
# Alpine based
=======
# Alpine based (3.14)
>>>>>>> 2e13b33fc5 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/docker/README.md

Alpine version:
* 3.15 for 3.5
* 3.16 for GDAL 3.6dev
=======
# Alpine based (3.15)
=======
# Alpine based (3.14)

## Ultra small: `osgeo/gdal:alpine-ultrasmall-latest`

* Image size: ~ 50 MB
* Raster drivers: VRT, GTiff, HFA, PNG, JPEG, MEM, JP2OpenJPEG, WEB, GPKG
* Vector drivers: Shapefile, MapInfo, VRT, Memory, GeoJSON, GPKG, SQLite
* External libraries enabled: libsqlite3, libproj, libcurl, libjpeg, libpng, libwebp, libzstd, libtiff (no LERC support at time of writing)
* No GDAL Python
* Base PROJ grid package
* Overall licensing terms of the GDAL build: permissive (X/MIT, BSD style, Apache, etc..)

See [alpine-ultrasmall/Dockerfile](alpine-ultrasmall/Dockerfile)
>>>>>>> 2e13b33fc5 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/docker/README.md
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9fdcdc669b (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> c71573c49d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
# Alpine based
=======
# Alpine based (3.14)
>>>>>>> 2e13b33fc5 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/docker/README.md
=======
>>>>>>> 9fdcdc669b (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> c71573c49d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
# Alpine based
<<<<<<< HEAD
>>>>>>> 311244091e (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
=======
=======
# Alpine based (3.14)
>>>>>>> 2e13b33fc5 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/docker/README.md
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)

Alpine version:
* 3.15 for 3.5
* 3.16 for GDAL 3.6dev
>>>>>>> 0f99b0ef18 (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
>>>>>>> 126d56369a (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
<<<<<<< HEAD
=======
>>>>>>> 9fdcdc669b (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 139f18cc33 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 311244091e (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])

## Small: `osgeo/gdal:alpine-small-latest`

* Image size: ~ 59 MB
* Raster drivers: ultrasmall + built-in + SQLite-based ones + network-based ones
* Vector drivers: ultrasmall + built-in + most XML-based ones + network-based ones + PostgreSQL
* Using internal libtiff and libgeotiff
* External libraries enabled: ultrasmall + libexpat, libpq, libssl
* *No* GDAL Python
* Base PROJ grid package (http://download.osgeo.org/proj/proj-datumgrid-1.8.zip)
* Overall licensing terms of the GDAL build: permissive (MIT, BSD style, Apache, etc..)

See [alpine-small/Dockerfile](alpine-small/Dockerfile)

## Normal: `osgeo/gdal:alpine-normal-latest`

* Image size: ~ 277 MB
* Raster drivers: small + netCDF, HDF5, BAG
* Vector drivers: small + Spatialite, XLS
* Using internal libtiff and libgeotiff
* External libraries enabled: small + libgeos, libhdf5, libhdf5, libkea, libnetcdf, libfreexl,
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
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 03b2261518 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 311244091e (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
=======
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 19e1deeb18 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD:docker/README.md
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate, libparquet
* GDAL Python
* Base PROJ grid package (http://download.osgeo.org/proj/proj-datumgrid-1.8.zip)
=======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 19e1deeb18 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
>>>>>>> 126d56369a (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
=======
<<<<<<< HEAD
>>>>>>> d8608c8f1e (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 8e8bcf6841 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
<<<<<<< HEAD
>>>>>>> 126d56369a (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
>>>>>>> 311244091e (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
=======
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
<<<<<<< HEAD
>>>>>>> 8e8bcf6841 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate
* GDAL Python (Python 3.9)
<<<<<<< HEAD
=======
<<<<<<< HEAD:docker/README.md
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> c71573c49d (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
<<<<<<< HEAD:docker/README.md
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate, libparquet
* GDAL Python
>>>>>>> 0f99b0ef18 (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
* Base PROJ grid package (http://download.osgeo.org/proj/proj-datumgrid-1.8.zip)
=======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9fdcdc669b (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 8e8bcf6841 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> 9fdcdc669b (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> OSGeo-master:docker/README.md
=======
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate
* GDAL Python (Python 3.9)
>>>>>>> c71573c49d (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> d8608c8f1e (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate
* GDAL Python (Python 3.9)
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 8e8bcf6841 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 19e1deeb18 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate
* GDAL Python (Python 3.9)
>>>>>>> accab8c0ec (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate
* GDAL Python (Python 3.9)
>>>>>>> 03b2261518 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
  libspatialite, libxml2, libpoppler, openexr, libheif, libdeflate, libparquet
* GDAL Python
>>>>>>> 0f99b0ef18 (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
>>>>>>> 311244091e (Docker: update Alpine based images to 3.16, and add Apache Arrow/Parquet to alpine-normal image [ci skip])
* Base PROJ grid package (http://download.osgeo.org/proj/proj-datumgrid-1.8.zip)
=======
>>>>>>> 9fdcdc669b (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 139f18cc33 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> d8608c8f1e (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> ed22ba7fba (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> b45f9ceac8 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 19e1deeb18 (Merge branch 'master' of github.com:OSGeo/gdal)
* Base PROJ grid package
>>>>>>> 2e13b33fc5 (Merge branch 'master' of github.com:OSGeo/gdal):gdal/docker/README.md
* Overall licensing terms of the GDAL build: copy-left (GPL) + LGPL + permissive

See [alpine-normal/Dockerfile](alpine-normal/Dockerfile)

# Ubuntu based

Ubuntu version:
* 20.04 for GDAL 3.4 and 3.5
* 22.04 for GDAL 3.6dev

## Small: `osgeo/gdal:ubuntu-small-latest`

* Image size: ~ 385 MB
* Raster drivers: all built-in + JPEG + PNG + JP2OpenJPEG + WEBP +SQLite-based ones + network-based ones
* Vector drivers: all built-in + XML based ones + SQLite-based ones + network-based ones + PostgreSQL
* Using internal libtiff and libgeotiff
* External libraries enabled: libsqlite3, libproj, libcurl, libjpeg, libpng, libwebp,
  libzstd, libexpat, libxerces-c, libpq, libssl, libgeos, libspatialite
* GDAL Python (Python 3.8 for Ubuntu 20.04, Python 3.10 for Ubuntu 22.04)
* Base PROJ grid package (http://download.osgeo.org/proj/proj-datumgrid-1.8.zip)
* Overall licensing terms of the GDAL build: LGPL + permissive (MIT, BSD style, Apache, etc..)

See [ubuntu-small/Dockerfile](ubuntu-small/Dockerfile)

## Full: `osgeo/gdal:ubuntu-full-latest` (aliased to `osgeo/gdal`)

* Image size: ~ 1.48 GB
* Raster drivers: all based on almost all possible free and open-source dependencies
* Vector drivers: all based on almost all possible free and open-source dependencies
* Using internal libtiff and libgeotiff
* External libraries enabled: small + libnetcdf, libhdf4, libhdf5, libtiledb, libkea,
  mongocxx 3.4, libspatialite, unixodbc, libxml2, libcfitsio, libmysqlclient,
  libkml, libpoppler, pdfium, openexr, libheif, libdeflate, libparquet
* GDAL Python (Python 3.8 for Ubuntu 20.04, Python 3.10 for Ubuntu 22.04)
* *All* PROJ grid packages (equivalent of latest of proj-data-X.zip from http://download.osgeo.org/proj/ at time of generation, > 500 MB)
* Overall licensing terms of the GDAL build: copy-left (GPL) + LGPL + permissive

See [ubuntu-full/Dockerfile](ubuntu-full/Dockerfile)


# Usage

Pull the required image and then run passing the gdal program you want to execute as a [docker run](https://docs.docker.com/engine/reference/commandline/run/) command. Bind a volume from your local file system to the docker container to run gdal programs that accept a file argument. For example, binding `-v /home:/home` on Linux or `-v /Users:/Users` on Mac will allow you to reference files in your home directory by passing their full path. Use the docker `--rm` option to automatically remove the container when the run completes.

Note: you should *not* try to install GDAL (directly or indirectly through other packages that depend on it) with the package managing system (apt/apk) of the Linux distributions. It will conflict with the custom GDAL version provided by the Docker image and will likely result in a broken container.

## Example:

```shell
docker pull osgeo/gdal:alpine-small-latest
docker run --rm -v /home:/home osgeo/gdal:alpine-small-latest gdalinfo $PWD/my.tif
```

# Images of releases

Tagged images of recent past releases are available. The last ones (at time of writing) are for GDAL 3.5.0 and PROJ 9.0.0, for linux/amd64 and linux/arm64:
* osgeo/gdal:alpine-small-3.5.0
* osgeo/gdal:alpine-normal-3.5.0
* osgeo/gdal:ubuntu-small-3.5.0
* osgeo/gdal:ubuntu-full-3.5.0

## Multi-arch Images

Each directory contains a `build.sh` shell script that supports building images
for multiple platforms using an experimental feature called [Docker BuildKit](https://docs.docker.com/buildx/working-with-buildx/).

BuildKit CLI looks like `docker buildx build` vs. `docker build`
and allows images to build not only for the architecture and operating system
that the user invoking the build happens to run, but for others as well.

There is a small setup process depending on your operating system. Refer to [Preparation toward running Docker on ARM Mac: Building multi-arch images with Docker BuildX](https://medium.com/nttlabs/buildx-multiarch-2c6c2df00ca2).

#### Example Scenario

If you're running Docker for MacOS with an Intel CPU
and you wanted to build the `alpine-small` image with support for Raspberry Pi 4,
adding a couple flags when running `alpine-small/build.sh` can greatly simplify this process

#### Enabling

Use the two script flags in order to leverage BuildKit:

| Flag  | Description | Arguments |
| ------------- | ------------- | ------------- |
| --with-multi-arch  | Will build using the `buildx` plugin  | N/A |
| --platform  | Which architectures to build  | linux/amd64,linux/arm64 |

**Example**

`alpine-small/build.sh --with-multi-arch --release --gdal v3.2.0 --proj master --platform linux/arm64,linux/amd64`

## Custom Base Image

Override the base image, used to build and run gdal, by setting the environment variable: `BASE_IMAGE`

**Example**

`BASE_IMAGE="debian:stable" ubuntu-small/build.sh --release --gdal v3.2.0 --proj master`

## Custom Image Names

Override the image and repository of the final image by setting the environment variable: `TARGET_IMAGE`

**Example**

`TARGET_IMAGE="YOU_DOCKER_USERNAME/gdal" alpine-small/build.sh --release --gdal v3.2.0 --proj master`
