##
# osgeo/gdal:ubuntu-full

# This file is available at the option of the licensee under:
# Public domain
# or licensed under X/MIT (LICENSE.TXT) Copyright 2019 Even Rouault <even.rouault@spatialys.com>

ARG BASE_IMAGE=ubuntu:20.04

FROM $BASE_IMAGE as builder

# Derived from osgeo/proj by Howard Butler <howard@hobu.co>
LABEL maintainer="Even Rouault <even.rouault@spatialys.com>"

# Setup build env for PROJ
USER root
RUN apt-get update -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --fix-missing --no-install-recommends \
            software-properties-common build-essential ca-certificates \
            git make cmake wget unzip libtool automake \
            zlib1g-dev libsqlite3-dev pkg-config sqlite3 libcurl4-gnutls-dev \
            libtiff5-dev \
    && rm -rf /var/lib/apt/lists/*

ARG JAVA_VERSION=11
# Setup build env for GDAL
RUN apt-get update -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --fix-missing --no-install-recommends \
       libcharls-dev libopenjp2-7-dev libcairo2-dev \
       python3-dev python3-numpy \
       libpng-dev libjpeg-dev libgif-dev liblzma-dev libgeos-dev \
       curl libxml2-dev libexpat-dev libxerces-c-dev \
       libnetcdf-dev libpoppler-dev libpoppler-private-dev \
       libspatialite-dev swig ant libhdf4-alt-dev libhdf5-serial-dev \
       libfreexl-dev unixodbc-dev libwebp-dev libepsilon-dev \
       liblcms2-2 libpcre3-dev libcrypto++-dev libdap-dev libfyba-dev \
       libkml-dev libmysqlclient-dev libogdi-dev \
       libcfitsio-dev openjdk-"$JAVA_VERSION"-jdk libzstd-dev \
       libpq-dev libssl-dev libboost-dev \
       autoconf automake bash-completion libarmadillo-dev \
       libopenexr-dev libheif-dev \
       libdeflate-dev \
    && rm -rf /var/lib/apt/lists/*

# Build likbkea
ARG KEA_VERSION=1.4.14
RUN wget -q https://github.com/ubarsc/kealib/archive/kealib-${KEA_VERSION}.zip \
    && unzip -q kealib-${KEA_VERSION}.zip \
    && rm -f kealib-${KEA_VERSION}.zip \
    && cd kealib-kealib-${KEA_VERSION} \
    && cmake . -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr -DHDF5_INCLUDE_DIR=/usr/include/hdf5/serial \
        -DHDF5_LIB_PATH=/usr/lib/x86_64-linux-gnu/hdf5/serial -DLIBKEA_WITH_GDAL=OFF \
    && make -j$(nproc) \
    && make install DESTDIR="/build_thirdparty" \
    && make install \
    && cd .. \
    && rm -rf kealib-kealib-${KEA_VERSION} \
    && for i in /build_thirdparty/usr/lib/*; do strip -s $i 2>/dev/null || /bin/true; done \
    && for i in /build_thirdparty/usr/bin/*; do strip -s $i 2>/dev/null || /bin/true; done

# Build mongo-c-driver
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
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
<<<<<<< HEAD
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 099258b658 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 708f304349 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG MONGO_C_DRIVER_VERSION=1.17.6
=======
ARG MONGO_C_DRIVER_VERSION=1.17.5
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> 145dd38d72 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6271648633 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG MONGO_C_DRIVER_VERSION=1.17.6
=======
ARG MONGO_C_DRIVER_VERSION=1.17.5
>>>>>>> 9457c72950 (Docker: update to use an alternate repo to OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> 30c9b12560 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
>>>>>>> a20945fa20 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> 30c9b12560 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> gdal-raster-parallelisation
=======
=======
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG MONGO_C_DRIVER_VERSION=1.17.6
=======
ARG MONGO_C_DRIVER_VERSION=1.17.5
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> feature/gdaltiles
=======
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> OSGeo-master
=======
<<<<<<< HEAD
=======
>>>>>>> feature/gdaltiles
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 6271648633 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
ARG MONGO_C_DRIVER_VERSION=1.17.5
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
=======
ARG MONGO_C_DRIVER_VERSION=1.17.5
>>>>>>> 120fe7f972 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> 9f0b95a795 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> 145dd38d72 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 099258b658 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> a20945fa20 (Docker: update to use an alternate repo to OSGeo/gdal)
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
=======
ARG MONGO_C_DRIVER_VERSION=1.17.6
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG MONGO_C_DRIVER_VERSION=1.17.5
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 708f304349 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
RUN mkdir mongo-c-driver \
    && wget -q https://github.com/mongodb/mongo-c-driver/releases/download/${MONGO_C_DRIVER_VERSION}/mongo-c-driver-${MONGO_C_DRIVER_VERSION}.tar.gz -O - \
        | tar xz -C mongo-c-driver --strip-components=1 \
    && cd mongo-c-driver \
    && mkdir build_cmake \
    && cd build_cmake \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_TESTS=NO -DCMAKE_BUILD_TYPE=Release \
    && make -j$(nproc) \
    && make install DESTDIR="/build_thirdparty" \
    && make install \
    && cd ../.. \
    && rm -rf mongo-c-driver \
    && rm /build_thirdparty/usr/lib/x86_64-linux-gnu/*.a \
    && for i in /build_thirdparty/usr/lib/x86_64-linux-gnu/*; do strip -s $i 2>/dev/null || /bin/true; done \
    && for i in /build_thirdparty/usr/bin/*; do strip -s $i 2>/dev/null || /bin/true; done

# Build mongocxx
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
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
<<<<<<< HEAD
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 099258b658 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 708f304349 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG MONGOCXX_VERSION=3.6.5
=======
ARG MONGOCXX_VERSION=3.6.3
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> 145dd38d72 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6271648633 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG MONGOCXX_VERSION=3.6.5
=======
ARG MONGOCXX_VERSION=3.6.3
>>>>>>> 9457c72950 (Docker: update to use an alternate repo to OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> 30c9b12560 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
>>>>>>> a20945fa20 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> 30c9b12560 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> gdal-raster-parallelisation
=======
=======
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG MONGOCXX_VERSION=3.6.5
=======
ARG MONGOCXX_VERSION=3.6.3
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> feature/gdaltiles
=======
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> OSGeo-master
=======
<<<<<<< HEAD
=======
>>>>>>> feature/gdaltiles
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 6271648633 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
ARG MONGOCXX_VERSION=3.6.3
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
=======
ARG MONGOCXX_VERSION=3.6.3
>>>>>>> 120fe7f972 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> 9f0b95a795 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> 145dd38d72 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 099258b658 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> a20945fa20 (Docker: update to use an alternate repo to OSGeo/gdal)
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
=======
ARG MONGOCXX_VERSION=3.6.5
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG MONGOCXX_VERSION=3.6.3
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 708f304349 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
RUN mkdir mongocxx \
    && wget -q https://github.com/mongodb/mongo-cxx-driver/archive/r${MONGOCXX_VERSION}.tar.gz -O - \
        | tar xz -C mongocxx --strip-components=1 \
    && cd mongocxx \
    && mkdir build_cmake \
    && cd build_cmake \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBSONCXX_POLY_USE_BOOST=ON -DMONGOCXX_ENABLE_SLOW_TESTS=NO -DCMAKE_BUILD_TYPE=Release -DBUILD_VERSION=${MONGOCXX_VERSION} \
    && make -j$(nproc) \
    && make install DESTDIR="/build_thirdparty" \
    && make install \
    && cd ../.. \
    && rm -rf mongocxx \
    && for i in /build_thirdparty/usr/lib/x86_64-linux-gnu/*; do strip -s $i 2>/dev/null || /bin/true; done \
    && for i in /build_thirdparty/usr/bin/*; do strip -s $i 2>/dev/null || /bin/true; done

# Build tiledb
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
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
<<<<<<< HEAD
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 099258b658 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 708f304349 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG TILEDB_VERSION=2.3.0
=======
ARG TILEDB_VERSION=2.2.7
>>>>>>> ad088f3587 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> 145dd38d72 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6271648633 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG TILEDB_VERSION=2.3.0
=======
ARG TILEDB_VERSION=2.2.7
>>>>>>> 9457c72950 (Docker: update to use an alternate repo to OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> 30c9b12560 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
>>>>>>> a20945fa20 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> 30c9b12560 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> gdal-raster-parallelisation
=======
=======
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
ARG TILEDB_VERSION=2.3.0
=======
ARG TILEDB_VERSION=2.2.7
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> feature/gdaltiles
=======
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
<<<<<<< HEAD
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> OSGeo-master
=======
<<<<<<< HEAD
=======
>>>>>>> feature/gdaltiles
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 6271648633 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
=======
ARG TILEDB_VERSION=2.2.7
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
=======
ARG TILEDB_VERSION=2.2.7
>>>>>>> 120fe7f972 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> 9f0b95a795 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 6bdd8a35a5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> 145dd38d72 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 099258b658 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> a20945fa20 (Docker: update to use an alternate repo to OSGeo/gdal)
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> 7494d4d891 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> c4eb45f967 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 9105b4f6b6 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 2b66f85bb3 (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
>>>>>>> 61413fe48c (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
=======
=======
ARG TILEDB_VERSION=2.3.0
>>>>>>> adab5a94f3 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 39cde30c78 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> a57f04675f (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 86346bde67 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 46c4af5ba5 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
ARG TILEDB_VERSION=2.2.7
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 708f304349 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
RUN mkdir tiledb \
    && wget -q https://github.com/TileDB-Inc/TileDB/archive/${TILEDB_VERSION}.tar.gz -O - \
        | tar xz -C tiledb --strip-components=1 \
    && cd tiledb \
    && mkdir build_cmake \
    && cd build_cmake \
    && ../bootstrap --prefix=/usr \
    && make -j$(nproc) \
    && make install-tiledb DESTDIR="/build_thirdparty" \
    && make install-tiledb \
    && cd ../.. \
    && rm -rf tiledb \
    && for i in /build_thirdparty/usr/lib/x86_64-linux-gnu/*; do strip -s $i 2>/dev/null || /bin/true; done \
    && for i in /build_thirdparty/usr/bin/*; do strip -s $i 2>/dev/null || /bin/true; done

# Build openjpeg
ARG OPENJPEG_VERSION=
RUN if test "${OPENJPEG_VERSION}" != ""; then ( \
    wget -q https://github.com/uclouvain/openjpeg/archive/v${OPENJPEG_VERSION}.tar.gz \
    && tar xzf v${OPENJPEG_VERSION}.tar.gz \
    && rm -f v${OPENJPEG_VERSION}.tar.gz \
    && cd openjpeg-${OPENJPEG_VERSION} \
    && cmake . -DBUILD_SHARED_LIBS=ON  -DBUILD_STATIC_LIBS=OFF -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
    && make -j$(nproc) \
    && make install \
    && mkdir -p /build_thirdparty/usr/lib/x86_64-linux-gnu \
    && rm -f /usr/lib/x86_64-linux-gnu/libopenjp2.so* \
    && mv /usr/lib/libopenjp2.so* /usr/lib/x86_64-linux-gnu \
    && cp -P /usr/lib/x86_64-linux-gnu/libopenjp2.so* /build_thirdparty/usr/lib/x86_64-linux-gnu \
    && for i in /build_thirdparty/usr/lib/x86_64-linux-gnu/*; do strip -s $i 2>/dev/null || /bin/true; done \
    && cd .. \
    && rm -rf openjpeg-${OPENJPEG_VERSION} \
    ); fi

# Install MDB Driver Jars
RUN wget -q https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/mdb-sqlite/mdb-sqlite-1.0.2.tar.bz2 \
  && tar -xjf mdb-sqlite-1.0.2.tar.bz2 \
  && mkdir -p /build/usr/share/java \
  && cp mdb-sqlite-1.0.2/lib/*.jar /build/usr/share/java \
  && rm -rf mdb-sqlite-1.0.2.tar.bz2 && rm -rf mdb-sqlite-1.0.2

#Set CLASSPATH so jars are found
ENV CLASSPATH="/build/usr/share/java/jackcess-1.1.14.jar:/build/usr/share/java/commons-logging-1.1.1.jar:/build/usr/share/java/commons-lang-2.4.jar"

#Build File Geodatabase

ARG WITH_FILEGDB=
RUN if echo "$WITH_FILEGDB" | grep -Eiq "^(y(es)?|1|true)$"  ; then ( \
  wget -q https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1.5.1/FileGDB_API_1_5_1-64gcc51.tar.gz \
  && tar -xzf FileGDB_API_1_5_1-64gcc51.tar.gz \
  && chown -R root:root FileGDB_API-64gcc51 \
  && mv FileGDB_API-64gcc51 /usr/local/FileGDB_API \
  && rm -rf /usr/local/FileGDB_API/lib/libstdc++* \
  && cp /usr/local/FileGDB_API/lib/* /build_thirdparty/usr/lib \
  && cp /usr/local/FileGDB_API/include/* /usr/include \
  && rm -rf FileGDB_API_1_5_1-64gcc51.tar.gz \
  ) ; fi


ARG WITH_PDFIUM=yes
RUN if echo "$WITH_PDFIUM" | grep -Eiq "^(y(es)?|1|true)$"  ; then ( \
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> OSGeo-master
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
  wget -q https://github.com/rouault/pdfium_build_gdal_3_4/releases/download/v1_pdfium_4538/install-ubuntu2004-rev4538.tar.gz \
  && tar -xzf install-ubuntu2004-rev4538.tar.gz \
  && chown -R root:root install \
  && mv install/lib/* /usr/lib/ \
  && mv install/include/* /usr/include/ \
  && rm -rf install-ubuntu2004-rev4538.tar.gz install \
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 120fe7f972 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
=======
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
>>>>>>> feature/gdaltiles
  wget -q https://github.com/rouault/pdfium_build_gdal_3_2/releases/download/v1_pdfium_4272/install-ubuntu2004-rev4272.tar.gz \
  && tar -xzf install-ubuntu2004-rev4272.tar.gz \
  && chown -R root:root install \
  && mv install/lib/* /usr/lib/ \
  && mv install/include/* /usr/include/ \
  && rm -rf install-ubuntu2004-rev4272.tar.gz install \
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> feature/gdaltiles
>>>>>>> 9457c72950 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
  wget -q https://github.com/rouault/pdfium_build_gdal_3_4/releases/download/v1_pdfium_4538/install-ubuntu2004-rev4538.tar.gz \
  && tar -xzf install-ubuntu2004-rev4538.tar.gz \
  && chown -R root:root install \
  && mv install/lib/* /usr/lib/ \
  && mv install/include/* /usr/include/ \
  && rm -rf install-ubuntu2004-rev4538.tar.gz install \
>>>>>>> 34342977ef (Merge branch 'master' of github.com:OSGeo/gdal)
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> OSGeo-master
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> gdal-raster-parallelisation
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 120fe7f972 (Docker: update to use an alternate repo to OSGeo/gdal)
=======
  wget -q https://github.com/rouault/pdfium_build_gdal_3_4/releases/download/v1_pdfium_4538/install-ubuntu2004-rev4538.tar.gz \
  && tar -xzf install-ubuntu2004-rev4538.tar.gz \
  && chown -R root:root install \
  && mv install/lib/* /usr/lib/ \
  && mv install/include/* /usr/include/ \
  && rm -rf install-ubuntu2004-rev4538.tar.gz install \
>>>>>>> ba1e26a07d (Merge branch 'master' of github.com:OSGeo/gdal)
=======
>>>>>>> 9457c72950 (Docker: update to use an alternate repo to OSGeo/gdal)
>>>>>>> 692d436bbd (Docker: update to use an alternate repo to OSGeo/gdal)
=======
>>>>>>> b486835dfc (Merge branch 'master' of github.com:OSGeo/gdal)
=======
=======
>>>>>>> 1c050736fa (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> 69ca760823 (Merge branch 'master' of github.com:OSGeo/gdal)
>>>>>>> feature/gdaltiles
  && apt-get update -y \
  && apt-get install -y --fix-missing --no-install-recommends liblcms2-dev \
  && rm -rf /var/lib/apt/lists/* \
  ) ; fi


RUN apt-get update -y \
    && apt-get install -y --fix-missing --no-install-recommends rsync ccache \
    && rm -rf /var/lib/apt/lists/*
ARG RSYNC_REMOTE

ARG WITH_DEBUG_SYMBOLS=no

# Refresh grids
ARG PROJ_DATUMGRID_LATEST_LAST_MODIFIED
ARG PROJ_INSTALL_PREFIX=/usr/local
COPY ./bh-proj.sh /buildscripts/bh-proj.sh
RUN DESTDIR=/build_tmp_proj /buildscripts/bh-proj.sh \
     && LD_LIBRARY_PATH=/build_tmp_proj/usr/local/lib /build_tmp_proj/usr/local/bin/projsync --target-dir /tmp/proj_grids --all \
     && rm -rf /build_tmp_proj

# Build PROJ
ARG PROJ_VERSION=master
RUN /buildscripts/bh-proj.sh

ARG GDAL_VERSION=master
ARG GDAL_RELEASE_DATE
ARG GDAL_BUILD_IS_RELEASE
ARG GDAL_REPOSITORY=OSGeo/gdal

# Build GDAL

COPY ./bh-gdal.sh /buildscripts/bh-gdal.sh
RUN /buildscripts/bh-gdal.sh

# Build final image
FROM $BASE_IMAGE as runner

USER root
RUN date
ARG JAVA_VERSION=11

RUN apt-get update \
# PROJ dependencies
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        libsqlite3-0 libtiff5 libcurl4 \
        wget curl unzip ca-certificates \
# GDAL dependencies
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        libcharls2 libopenjp2-7 libcairo2 python3-numpy \
        libpng16-16 libjpeg-turbo8 libgif7 liblzma5 libgeos-3.8.0 libgeos-c1v5 \
        libxml2 libexpat1 \
        libxerces-c3.2 libnetcdf-c++4 netcdf-bin libpoppler97 libspatialite7 gpsbabel \
        libhdf4-0-alt libhdf5-103 libhdf5-cpp-103 poppler-utils libfreexl1 unixodbc libwebp6 \
        libepsilon1 liblcms2-2 libpcre3 libcrypto++6 libdap25 libdapclient6v5 libfyba0 \
        libkmlbase1 libkmlconvenience1 libkmldom1 libkmlengine1 libkmlregionator1 libkmlxsd1 \
        libmysqlclient21 libogdi4.1 libcfitsio8 openjdk-"$JAVA_VERSION"-jre \
        libzstd1 bash bash-completion libpq5 libssl1.1 \
        libarmadillo9 libpython3.8 libopenexr24 libheif1 \
        libdeflate0 \
        python-is-python3 \
    # Workaround bug in ogdi packaging
    && ln -s /usr/lib/ogdi/libvrf.so /usr/lib \
    && rm -rf /var/lib/apt/lists/*

# Attempt to order layers starting with less frequently varying ones

COPY --from=builder  /build_thirdparty/usr/ /usr/

ARG PROJ_INSTALL_PREFIX=/usr/local
COPY --from=builder  /tmp/proj_grids/* ${PROJ_INSTALL_PREFIX}/share/proj/

COPY --from=builder  /build${PROJ_INSTALL_PREFIX}/share/proj/ ${PROJ_INSTALL_PREFIX}/share/proj/
COPY --from=builder  /build${PROJ_INSTALL_PREFIX}/include/ ${PROJ_INSTALL_PREFIX}/include/
COPY --from=builder  /build${PROJ_INSTALL_PREFIX}/bin/ ${PROJ_INSTALL_PREFIX}/bin/
COPY --from=builder  /build${PROJ_INSTALL_PREFIX}/lib/ ${PROJ_INSTALL_PREFIX}/lib/

COPY --from=builder  /build/usr/share/java /usr/share/java
COPY --from=builder  /build/usr/share/gdal/ /usr/share/gdal/
COPY --from=builder  /build/usr/include/ /usr/include/
COPY --from=builder  /build_gdal_python/usr/ /usr/
COPY --from=builder  /build_gdal_version_changing/usr/ /usr/

RUN ldconfig

#Set CLASSPATH so jars are found
ENV CLASSPATH="/usr/share/java/jackcess-1.1.14.jar:/usr/share/java/commons-logging-1.1.1.jar:/usr/share/java/commons-lang-2.4.jar"
