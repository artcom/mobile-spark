# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

cmake_minimum_required(VERSION 2.8)

set(APP_NAME mobile-spark)
project(${APP_NAME})

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

set(core_SOURCE_DIR ${PROJECT_SOURCE_DIR}/core)

add_subdirectory(acmake)
find_package(AcMake)


set(REQUIRED_PACKAGES Boost Cml libXML2 libCurl)
foreach(PACKAGE ${REQUIRED_PACKAGES})
    find_package(${PACKAGE})
endforeach()

add_subdirectory(core)

#add_library(LIBCURL STATIC IMPORTED)
#set_property(TARGET LIBCURL PROPERTY
#            IMPORTED_LOCATION ${libCurl_LIBRARY_DIRS})

if(ANDROID)
    # add imported target
    add_library(LIBXML2 STATIC IMPORTED)
    # point the imported target at the real file
    set_property(TARGET LIBXML2 PROPERTY
                IMPORTED_LOCATION ${LIBXML2_LIBRARY_DIRS})
elseif(IOS)
    add_subdirectory(iOS)
endif()

