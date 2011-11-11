# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

SET(TARGET_PLATFORM iPhoneOS)
SET(IOS True)

# SDK Info
SET(DEVROOT "/Developer/Platforms/${TARGET_PLATFORM}.platform/Developer")

#Gather all available SDK-paths and select latest
FILE(GLOB SDK_PATHS ${DEVROOT}/SDKs/*)
LIST(LENGTH SDK_PATHS length)

FOREACH(path ${SDK_PATHS})
	#MESSAGE ("Found SDK ${path}")
	SET(latest_SDK ${path})
ENDFOREACH(path)

#MESSAGE("Found ${length} SDKs -- latest is ${latest_SDK}")

#Make sure SDK is valid
find_path(SDKROOT "usr/include/stdlib.h" PATHS ${latest_SDK} NO_CMAKE_FIND_ROOT_PATH)

SET(CMAKE_OSX_SYSROOT "${SDKROOT}")
SET(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "")

#set target device: "1" -> iPhone, "2" -> iPad, "1,2 " -> both (remember the <whitespace> after the '2' !!!)
SET(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1")

SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_OSX_ARCHITECTURES armv7)

# Skip the platform compiler checks for cross compiling
SET(CMAKE_CXX_COMPILER_WORKS TRUE)
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET( CMAKE_C_FLAGS "-arch armv7 -mthumb ${CMAKE_C_FLAGS}" CACHE STRING "c flags" )
SET( CMAKE_CXX_FLAGS "-arch armv7 -mthumb ${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags" )

# Flags
ADD_DEFINITIONS("-no-cpp-precomp")
ADD_DEFINITIONS("--sysroot=${SDKROOT}")
ADD_DEFINITIONS("-miphoneos-version-min=${SDKVER}")

# Header
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1/armv7-apple-darwin10")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/System/Library/Frameworks")

# System Libraries
LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/System/Library/Frameworks")
LINK_DIRECTORIES("${SDKROOT}/usr/lib/gcc/arm-apple-darwin10/4.2.1")

SET (CMAKE_FIND_ROOT_PATH "${SDKROOT}")
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET (CMAKE_CROSSCOMPILING 1)
