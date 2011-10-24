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

set(EXTERNS Boost libXML2 Cml masl mar animation spark)
if (ANDROID) 
    list(APPEND EXTERNS android)
else (ANDROID)
    list(APPEND EXTERNS ios)
endif(ANDROID)

ac_add_library(
    ${APP_NAME} ${APP_NAME}
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS ${DEPENDS}
    EXTERNS ${EXTERNS}
)

