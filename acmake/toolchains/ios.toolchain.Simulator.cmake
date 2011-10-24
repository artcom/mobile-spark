# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

SET(TARGET_PLATFORM iPhoneSimulator)
SET(IOS True)

# SDK Info
SET (SDKVER "4.3")
SET (DEVROOT "/Developer/Platforms/${TARGET_PLATFORM}.platform/Developer")
SET (SDKROOT "${DEVROOT}/SDKs/${TARGET_PLATFORM}${SDKVER}.sdk")
SET (CMAKE_OSX_SYSROOT "${SDKROOT}")
SET (MACOSX_DEPLOYMENT_TARGET "Compiler Default")

SET(CMAKE_SYSTEM_PROCESSOR i386)
SET(CMAKE_OSX_ARCHITECTURES i386)

# Skip the platform compiler checks for cross compiling
#set (CMAKE_CXX_COMPILER_WORKS TRUE)
#set (CMAKE_C_COMPILER_WORKS TRUE)

# C Compiler
SET (CMAKE_C_COMPILER "${DEVROOT}/usr/bin/gcc-4.2")
SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG=1 -ggdb")
SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -ggdb")

# CXX Compiler
SET (CMAKE_CXX_COMPILER "${DEVROOT}/usr/bin/g++-4.2")
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DDEBUG=1 -ggdb")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -ggdb")

SET( CMAKE_C_FLAGS "-m32 ${CMAKE_C_FLAGS}" CACHE STRING "c flags" )
SET( CMAKE_CXX_FLAGS "-m32 ${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags" )

# Flags
ADD_DEFINITIONS("-m32")
ADD_DEFINITIONS("-arch i386")
ADD_DEFINITIONS("-no-cpp-precomp")
ADD_DEFINITIONS("--sysroot=${SDKROOT}")
ADD_DEFINITIONS("-miphoneos-version-min=${SDKVER}")

INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include")
#INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/System/Library/Frameworks")
#INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1/i686-apple-darwin10")

# System Libraries
LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/System/Library/Frameworks")
#LINK_DIRECTORIES("${SDKROOT}/usr/lib/system")
#LINK_DIRECTORIES("${SDKROOT}/usr/lib/gcc/i686-apple-darwin10/4.2.1")

SET (CMAKE_FIND_ROOT_PATH "${SDKROOT}")
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET (CMAKE_CROSSCOMPILING 0)
