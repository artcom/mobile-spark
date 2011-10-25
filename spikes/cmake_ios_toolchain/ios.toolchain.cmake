# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#SET(TARGET_PLATTFORM iPhoneOS)
SET(TARGET_PLATTFORM iPhoneSimulator)

# SDK Info
SET (SDKVER "4.3")
SET (DEVROOT "/Developer/Platforms/${TARGET_PLATTFORM}.platform/Developer")
SET (SDKROOT "${DEVROOT}/SDKs/${TARGET_PLATTFORM}${SDKVER}.sdk")
SET (CMAKE_OSX_SYSROOT "${SDKROOT}")

IF(${TARGET_PLATTFORM} STREQUAL iPhoneOS)
	SET(CMAKE_OSX_ARCHITECTURES armv7)
	SET (CMAKE_SYSTEM_PROCESSOR armv7)
ELSEIF(${TARGET_PLATTFORM} STREQUAL iPhoneSimulator)
	SET(CMAKE_OSX_ARCHITECTURES i686)
	#SET (CMAKE_SYSTEM_PROCESSOR armv7)

ENDIF()

SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

# C Compiler
#SET (CMAKE_C_COMPILER "${DEVROOT}/usr/bin/arm-apple-darwin10-gcc-4.2.1")
SET (CMAKE_C_COMPILER "${DEVROOT}/usr/bin/gcc-4.2")
SET (LINK_FLAGS "-arch i686")
#SET (CMAKE_C_LINK_EXECUTABLE "${DEVROOT}/usr/bin/g++-4.2")
#SET (CMAKE_C_FLAGS "-x objective-c")
SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG=1 -ggdb")
SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -ggdb")

# CXX Compiler
#SET (CMAKE_CXX_COMPILER "${DEVROOT}/usr/bin/arm-apple-darwin10-g++-4.2.1")
SET (CMAKE_CXX_COMPILER "${DEVROOT}/usr/bin/g++-4.2")
#SET (CMAKE_CXX_FLAGS "-x objective-c++")
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DDEBUG=1 -ggdb")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -ggdb")

include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(gcc-4.2 GNU)
CMAKE_FORCE_CXX_COMPILER(g++-4.2 GNU)

# Definitions

IF(${TARGET_PLATTFORM} STREQUAL iPhoneOS)
	ADD_DEFINITIONS("-arch armv7")
ELSEIF(${TARGET_PLATTFORM} STREQUAL iPhoneSimulator)
	ADD_DEFINITIONS("-arch i686")
ENDIF()
ADD_DEFINITIONS("-pipe")
ADD_DEFINITIONS("-no-cpp-precomp")
ADD_DEFINITIONS("--sysroot=${SDKROOT}")
ADD_DEFINITIONS("-miphoneos-version-min=${SDKVER}")

# normaly we do not need that definition, since the Compiler detects ObjectiveC++ automatically
#ADD_DEFINITIONS("-x objective-c++")

IF(${TARGET_PLATTFORM} STREQUAL iPhoneOS)
	set(CMAKE_EXE_LINKER_FLAGS "-arch armv7 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER} -L${SDKROOT}/usr/lib -L${SDKROOT}/usr/lib/system")
ELSEIF(${TARGET_PLATTFORM} STREQUAL iPhoneSimulator)
	set(CMAKE_EXE_LINKER_FLAGS "-arch i686 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER} -L${SDKROOT}/usr/lib -L${SDKROOT}/usr/lib/system")
ENDIF()

SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1")
#INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include/c++/4.2.1/armv7-apple-darwin10")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/opt/iphone-${SDKVER}/include")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/local/iphone-${SDKVER}/include")

# System Libraries
LINK_DIRECTORIES("${SDKROOT}/usr/lib")
#LINK_DIRECTORIES("${SDKROOT}/usr/lib/gcc/arm-apple-darwin10/4.2.1/")
#LINK_DIRECTORIES("${SDKROOT}/opt/iphone-${SDKVER}/lib")
#LINK_DIRECTORIES("${SDKROOT}/usr/local/iphone-${SDKVER}/lib")

SET (CMAKE_CROSSCOMPILING 1)