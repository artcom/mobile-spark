# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

file(GLOB SOURCES *.m *.mm)
file(GLOB HEADERS *.h *.pch)

set(EXTERNS libCurl Boost Cml libXML2)

ac_add_library(
    SparkViewerBase SparkViewerBase
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS spark
)

#Apple ios Frameworks
ADD_FRAMEWORK(SparkViewerBase UIKit)
ADD_FRAMEWORK(SparkViewerBase Foundation)
ADD_FRAMEWORK(SparkViewerBase QuartzCore)
ADD_FRAMEWORK(SparkViewerBase OpenGLES)
ADD_FRAMEWORK(SparkViewerBase CoreMotion)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${libXML2_INCLUDE_DIRS})
include_directories(${Cml_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/core/src)
    
