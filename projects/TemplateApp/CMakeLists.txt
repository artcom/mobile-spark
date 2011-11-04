# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

cmake_minimum_required(VERSION 2.8)

if(IOS)
    set(APP_NAME Template)
else(IOS)
    set(APP_NAME TemplateApp)
endif(IOS)	
project(${APP_NAME})

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

set(core_SOURCE_DIR ../../core)

add_subdirectory(../../acmake "${CMAKE_CURRENT_BINARY_DIR}/acmake")
find_package(AcMake)

set(spark_DIR ../../core/src/spark)
find_package(spark)

set(REQUIRED_PACKAGES Boost Cml libXML2 libCurl)
foreach(PACKAGE ${REQUIRED_PACKAGES})
    find_package(${PACKAGE})
endforeach()


include_directories(../../core/src)
add_subdirectory(src)
if(IOS)
    add_subdirectory(ios)
endif()
