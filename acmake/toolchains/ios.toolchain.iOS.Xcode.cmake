# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

SET (CMAKE_CROSSCOMPILING 1)

SET(TARGET_PLATFORM iPhoneOS)

SET(IOS_SDK_VERSION "5.1")
SET(IOS_DEPLOY_TGT "5.0")

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

#SET(CMAKE_OSX_SYSROOT "${SDKROOT}")

if(XCODE)
    set(CMAKE_OSX_SYSROOT "iphoneos${IOS_SDK_VERSION}" CACHE STRING "SDK version" FORCE)
else()
    set(CMAKE_OSX_SYSROOT ${SDKROOT} CACHE STRING "SDK version" FORCE)
endif()

SET(CMAKE_CXX_COMPILER_WORKS TRUE)
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET ${IOS_DEPLOY_TGT})

#set target device: "1" -> iPhone, "2" -> iPad, "1,2 " -> both (remember the <whitespace> after the '2' !!!)
SET(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2 ")

SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_OSX_ARCHITECTURES armv7)

# Skip the platform compiler checks for cross compiling

SET (CMAKE_C_COMPILER             "${DEVROOT}/usr/bin/clang")
SET (CMAKE_C_FLAGS_DEBUG          "-g -x objective-c")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG -x objective-c")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG -x objective-c")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -x objective-c")

SET (CMAKE_CXX_COMPILER             "${DEVROOT}/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g -x objective-c++")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG -x objective-c++")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG -x objective-c++")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -x objective-c++")

SET (CMAKE_AS      "${DEVROOT}/usr/bin/as")
SET (CMAKE_LINKER  "${DEVROOT}/usr/bin/ld")
SET (CMAKE_NM      "${DEVROOT}/usr/bin/nm")
SET (CMAKE_RANLIB  "${DEVROOT}/usr/bin/ranlib")

# Flags
SET(OUR_FLAGS="-arch armv7  -Wall -pipe -no-cpp-precomp -isysroot $SDKROOT -miphoneos-version-min=$IOS_DEPLOY_TGT -I${SDKROOT}/usr/include/")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OUR_FLAGS}" CACHE STRING "c flags")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OUR_FLAGS}" CACHE STRING "c++ flags")

set(CMAKE_EXE_LINKER_FLAGS
    "-framework OpenGLES -framework AudioToolbox -framework CoreGraphics -framework QuartzCore -framework UIKit"
)

# Header
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include")
#INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1")
#INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1/armv7-apple-darwin10")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/System/Library/Frameworks")

# System Libraries
LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/System/Library/Frameworks")
LINK_DIRECTORIES("${SDKROOT}/usr/lib/gcc/arm-apple-darwin10/4.2.1")

set(CMAKE_FIND_ROOT_PATH ${DEVROOT} ${SDKROOT})

#SET (CMAKE_FIND_ROOT_PATH "${SDKROOT}")
#SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
#SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#MESSAGE("CMAKE_OSX_SYSROOT: ${CMAKE_OSX_SYSROOT}")
