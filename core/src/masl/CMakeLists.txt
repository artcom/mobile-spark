# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)
file(GLOB NETWORKING_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/networking/*.cpp")
file(GLOB NETWORKING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/networking/*.h")
list(APPEND SOURCES ${NETWORKING_SOURCES})
list(APPEND HEADERS ${NETWORKING_HEADERS})

set(EXTERNS Boost Cml libXML2 libCurl)
if(ANDROID)
    find_library(log_LIBRARY_DIRS log )
    list(APPEND EXTERNS log)
endif(ANDROID)

ac_add_library(
    masl masl
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
)

if (ACMAKE_BUILD_TESTS)
    add_subdirectory(Tests)
endif (ACMAKE_BUILD_TESTS)
