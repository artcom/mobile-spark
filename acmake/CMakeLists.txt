# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


project(AcMake)

# installation paths for our files
set(ACMAKE_MODULES_SUB lib/acmake/cmake)

# determine absolute build time locations of our files
set(ACMAKE_INSTALLED_MODULES_DIR ${CMAKE_INSTALL_PREFIX}/${ACMAKE_MODULES_SUB})

# generate the config script
configure_file(AcMakeConfig.cmake.in ${CMAKE_BINARY_DIR}/${ACMAKE_MODULES_SUB}/AcMakeConfig.cmake @ONLY)

# install config file
install(
    FILES ${CMAKE_BINARY_DIR}/${ACMAKE_MODULES_SUB}/AcMakeConfig.cmake
    DESTINATION ${ACMAKE_MODULES_SUB}
)

# install modules
install(
    DIRECTORY modules/
    DESTINATION ${ACMAKE_MODULES_SUB}
    FILES_MATCHING
        PATTERN "*.cmake"
        PATTERN ".svn" EXCLUDE
)

