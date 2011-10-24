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
if(ANDROID)
    find_library(GLES20_LIBRARY_DIRS GLESv2)
    list(APPEND DEPENDS android)
    list(APPEND EXTERNS GLES20)
endif(ANDROID)

ac_add_library(
    mar mar
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl png
)
if(IOS)
    ADD_FRAMEWORK(mar OpenGLES)
endif(IOS)


