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

set(EXTERNS Boost Cml libXML2)
set(DEPENDS animation mar)

if(ANDROID)
    list(APPEND DEPENDS android)
else(ANDROID)
     list(APPEND DEPENDS iOS)
endif(ANDROID)     

ac_add_library(
    spark spark
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS ${DEPENDS}
)

if(IOS)
    ADD_FRAMEWORK(spark OpenGLES)
endif(IOS)

if (ACMAKE_BUILD_TESTS)
    add_subdirectory(Tests)
endif (ACMAKE_BUILD_TESTS)
