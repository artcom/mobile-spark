# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

cmake_minimum_required(VERSION 2.8)

set(APP_NAME ACProjectView)
project(${APP_NAME})

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

set(core_SOURCE_DIR "$ENV{MOBILE_SPARK}/core")

add_subdirectory("$ENV{MOBILE_SPARK}/acmake" "${CMAKE_CURRENT_BINARY_DIR}/acmake")
find_package(AcMake)

set(spark_DIR "$ENV{MOBILE_SPARK}/core/src/spark")
find_package(spark)

#when spark is found library output directory should be set from core output dir to project output dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../lib/${ARMEABI_NDK_NAME}")

set(REQUIRED_PACKAGES Boost Cml libXML2 libCurl)
foreach(PACKAGE ${REQUIRED_PACKAGES})
    find_package(${PACKAGE})
endforeach()


include_directories("$ENV{MOBILE_SPARK}/core/src")
add_subdirectory(src)
if(IOS)
    add_subdirectory(ios)
endif()

