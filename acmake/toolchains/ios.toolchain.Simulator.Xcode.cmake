SET(TARGET_PLATFORM iPhoneSimulator)
SET(IOS True)

# SDK Info
SET(SDKVER "4.3")
SET(DEVROOT "/Developer/Platforms/${TARGET_PLATFORM}.platform/Developer")
SET(SDKROOT "${DEVROOT}/SDKs/${TARGET_PLATFORM}${SDKVER}.sdk")
SET(CMAKE_OSX_SYSROOT "${SDKROOT}")
SET(MACOSX_DEPLOYMENT_TARGET "Compiler Default")

SET(CMAKE_SYSTEM_PROCESSOR i386)
SET(CMAKE_OSX_ARCHITECTURES i386)

# Skip the platform compiler checks for cross compiling 
SET(CMAKE_CXX_COMPILER_WORKS TRUE)
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET( CMAKE_C_FLAGS "-m32 ${CMAKE_C_FLAGS}" CACHE STRING "c flags" )
SET( CMAKE_CXX_FLAGS "-m32 ${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags" )

# Flags
ADD_DEFINITIONS("-arch i386")
ADD_DEFINITIONS("-no-cpp-precomp")
ADD_DEFINITIONS("--sysroot=${SDKROOT}")
ADD_DEFINITIONS("-miphoneos-version-min=${SDKVER}")

# Header
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
