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

set(EXTERNS Boost)

ac_add_library(
    animation animation
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl
)
if (ACMAKE_BUILD_TESTS)
    add_subdirectory(Tests)
endif (ACMAKE_BUILD_TESTS)

