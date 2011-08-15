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
	SET(CMAKE_OSX_ARCHITECTURES i386)
ENDIF()

# Skip the platform compiler checks for cross compiling 
set (CMAKE_CXX_COMPILER_WORKS TRUE)
set (CMAKE_C_COMPILER_WORKS TRUE)

# C Compiler
SET (CMAKE_C_COMPILER "${DEVROOT}/usr/bin/gcc-4.2")

# CXX Compiler
SET (CMAKE_CXX_COMPILER "${DEVROOT}/usr/bin/g++-4.2")

# Definitions
#builds a 32bit app
ADD_DEFINITIONS("-m32")
ADD_DEFINITIONS("-arch i386")
ADD_DEFINITIONS("-no-cpp-precomp")
ADD_DEFINITIONS("--sysroot=${SDKROOT}")
ADD_DEFINITIONS("-miphoneos-version-min=${SDKVER}")

INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/usr/include")
INCLUDE_DIRECTORIES(SYSTEM "${SDKROOT}/System/Library/Frameworks")

# System Libraries
LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/System/Library/Frameworks")
LINK_DIRECTORIES("${DEVROOT}/usr/lib/gcc/i686-apple-darwin10/4.2.1")

SET (CMAKE_FIND_ROOT_PATH "${SDKROOT}")
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET (CMAKE_CROSSCOMPILING 1)