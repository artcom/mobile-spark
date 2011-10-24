# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

if(ANDROID)
    find_path(libCurl_INCLUDE_DIR curl/curl.h ${core_SOURCE_DIR}/netsrc/libcurl/android NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
    find_library(libCurl_LIBRARY_DIR libcurl.a ${core_SOURCE_DIR}/netsrc/libcurl/android NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
else(ANDROID)
    find_path(libCurl_INCLUDE_DIR NAMES curl/curl.h PATHS ${core_SOURCE_DIR}/netsrc/libcurl/ios/ NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
    find_library(libCurl_LIBRARY_DIR libcurl.a ${core_SOURCE_DIR}/netsrc/libcurl/ios NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
    include_directories(${libCurl_INCLUDE_DIR})
endif()

set(libCurl_LIBRARY_DIRS ${libCurl_LIBRARY_DIR})
set(libCurl_INCLUDE_DIRS ${libCurl_INCLUDE_DIR})

