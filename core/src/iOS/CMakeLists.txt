# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

file(GLOB SOURCES *.cpp *.mm *.m)
file(GLOB HEADERS *.h)

set(EXTERNS Boost)

ac_add_library(
    iOS iOS
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl mar png
)

ADD_FRAMEWORK(iOS CoreGraphics)
ADD_FRAMEWORK(iOS QuartzCore)
ADD_FRAMEWORK(iOS CoreText)
ADD_FRAMEWORK(iOS CoreMedia)
ADD_FRAMEWORK(iOS AVFoundation)
