# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

SET(TARGET_PLATFORM iPhoneOS)

SET(IOS_DEPLOY_TGT "4.2")

SET(IOS True)

# SDK Info
SET(DEVROOT "/Applications/Xcode.app/Contents/Developer/Platforms/${TARGET_PLATFORM}.platform/Developer")

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
SET(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${IOS_DEPLOY_TGT})

#set target device: "1" -> iPhone, "2" -> iPad, "1,2 " -> both (remember the <whitespace> after the '2' !!!)
SET(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2 ")

SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_OSX_ARCHITECTURES armv7)

# Skip the platform compiler checks for cross compiling
SET(CMAKE_CXX_COMPILER_WORKS TRUE)
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET (CMAKE_C_COMPILER             "${DEVROOT}/usr/bin/clang")
SET (CMAKE_C_FLAGS_DEBUG          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (CMAKE_CXX_COMPILER             "${DEVROOT}/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

#SET (CMAKE_AR      "${DEVROOT}/usr/bin/ar")
SET (CMAKE_AS      "${DEVROOT}/usr/bin/as")
SET (CMAKE_LINKER  "${DEVROOT}/usr/bin/ld")
SET (CMAKE_NM      "${DEVROOT}/usr/bin/nm")
SET (CMAKE_RANLIB  "${DEVROOT}/usr/bin/ranlib")

# Flags
SET(OUR_FLAGS="-arch armv7 -mthumb -Wall -pipe -no-cpp-precomp -isysroot $SDKROOT -miphoneos-version-min=$IOS_DEPLOY_TGT -I$SDKROOT/usr/include/")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OUR_FLAGS}" CACHE STRING "c flags")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OUR_FLAGS}" CACHE STRING "c++ flags")

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
