# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

if(ANDROID)
    find_path(libXML2_INCLUDE_DIR NAMES libxml/xpath.h PATHS ${core_SOURCE_DIR}/netsrc/libxml2/includes/ NO_CMAKE_FIND_ROOT_PATH)
    find_library(libXML2_LIBRARY_DIR libxml2.a ${core_SOURCE_DIR}/netsrc/libxml2/ NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
else(ANDROID)
    find_path(libXML2_INCLUDE_DIR NAMES libxml/parser.h PATHS ${CMAKE_OSX_SYSROOT}/usr/include/libxml2 NO_CMAKE_FIND_ROOT_PATH)
    find_library(libXML2_LIBRARY_DIR libxml2.dylib ${CMAKE_OSX_SYSROOT}/usr/lib/ NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
endif()
set(libXML2_LIBRARY_DIRS ${libXML2_LIBRARY_DIR})
set(libXML2_INCLUDE_DIRS ${libXML2_INCLUDE_DIR})
