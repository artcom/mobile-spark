# ----------------------------------------------------------------------------
#  Android CMake toolchain file, for use with the ndk r5
#  See home page: http://code.google.com/p/android-cmake/
#
#  Usage Linux:
#   $ export ANDROID_NDK=/<absolute path to NDK>
#   $ cmake -DCMAKE_TOOLCHAIN_FILE=<path to this file>/android.toolchain.cmake ..
#   $ make
#
#  Usage Linux (using standalone toolchain):
#   $ export ANDROID_NDK_TOOLCHAIN_ROOT=/<absolute path to standalone toolchain>
#   $ cmake -DCMAKE_TOOLCHAIN_FILE=<path to this file>/android.toolchain.cmake ..
#   $ make
#
#  Usage Windows:
#     You need native port of make to build your project.
#     For example this one: http://gnuwin32.sourceforge.net/packages/make.htm
#
#   $ SET ANDROID_NDK=C:\<absolute path to NDK>\android-ndk-r5b
#   $ cmake.exe -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=<path to this file>\android.toolchain.cmake -DCMAKE_MAKE_PROGRAM=C:\<absolute path to make>\make.exe ..
#   $ C:\<absolute path to make>\make.exe
#
#
#  Toolchain options (can be set as cmake parameters: -D<option_name>=<value>):
#    ANDROID_NDK=/opt/android-ndk-r5b - path to NDK root.
#      Can be set as environment variable.
#
#    ANDROID_NDK_TOOLCHAIN_ROOT=/opt/android-toolchain - path to standalone toolchain.
#      Option is not used if full NDK is found. Can be set as environment variable.
#
#    ANDROID_LEVEL=android-8 - level of android API to use.
#      Option is ignored when build uses stanalone toolchain.
#
#    ARM_TARGET=armeabi-v7a - type of floating point support.
#      Other possible values are: "armeabi", "armeabi-v7a with NEON", "armeabi-v7a with VFPV3"
#
#    FORCE_ARM=false - set true to generate 32-bit ARM instructions instead of Thumb-1.
#
#    NO_UNDEFINED=true - set true to show all undefined symbols will as linker errors even if they are not used.
#
#    NO_SWIG=false - set true to disable SWIG package
#
#
#  Toolcahin will search for NDK/toolchain in following order:
#    ANDROID_NDK - cmake parameter
#    ANDROID_NDK - environment variable
#    ANDROID_NDK - default location
#    ANDROID_NDK_TOOLCHAIN_ROOT - cmake parameter
#    ANDROID_NDK_TOOLCHAIN_ROOT - environment variable
#    ANDROID_NDK_TOOLCHAIN_ROOT - default location
#
#
#  What?:
#     Make sure to do the following in your scripts:
#       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${my_cxx_flags}")
#       SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ${my_cxx_flags}")
#       The flags will be prepopulated with critical flags, so don't loose them.
#    
#     ANDROID and BUILD_ANDROID will be set to true, you may test these 
#     variables to make necessary changes.
#    
#     Also ARMEABI and ARMEABI_V7A will be set true, mutually exclusive. V7A is
#     for floating point. NEON option will be set true if fpu is set to neon.
#
#     LIBRARY_OUTPUT_PATH_ROOT should be set in cache to determine where android
#     libraries will be installed.
#        default is ${CMAKE_SOURCE_DIR} , and the android libs will always be
#        under ${LIBRARY_OUTPUT_PATH_ROOT}/libs/armeabi* depending on target.
#        this will be convenient for android linking
#
#     Base system is Linux, but you may need to change things 
#     for android compatibility.
#   
#
#   - initial version December 2010 Ethan Rublee ethan.ruble@gmail.com
#   - modified April 2011 Andrey Kamaev andrey.kamaev@itseez.com
#     [+] added possibility to build with NDK (without standalone toolchain)
#     [+] support croos compilation on Windows (native, no cygwin support)
#     [+] added compiler option to force "char" type to be signed
#     [+] added toolchain option to compile to 32-bit ARM instructions
#     [+] added toolchain option to disable SWIG search
#     [+] added platform "armeabi-v7a with VFPV3"
#     [~] ARM_TARGETS renamed to ARM_TARGET
#   - modified April 2011 Andrey Kamaev andrey.kamaev@itseez.com
#     [+] EXECUTABLE_OUTPUT_PATH is set by toolchain (required on Windows)
#     [~] Fixed bug with ANDROID_LEVEL variable
#     [~] turn off SWIG search if it is not found first time
# ----------------------------------------------------------------------------

# this one is important
set( CMAKE_SYSTEM_NAME Linux )

set( ANDROID_NDK_DEFAULT_SEARCH_PATH /opt/android-ndk-r6 )
set( ANDROID_NDK_TOOLCHAIN_DEFAULT_SEARCH_PATH /opt/android-toolchain )
set( TOOL_OS_SUFFIX "" )

if( APPLE )
    set( NDKSYSTEM "darwin-x86" )
elseif( WIN32 )
    set( NDKSYSTEM "windows" )
    set( TOOL_OS_SUFFIX ".exe" )
elseif( UNIX )
    set( NDKSYSTEM "linux-x86" )
else()
    message( FATAL_ERROR "Your platform is not supported" )
endif()

#look for android NDK
if( NOT DEFINED ANDROID_NDK )
    set( ANDROID_NDK $ENV{ANDROID_NDK} )
endif()

if( NOT EXISTS ${ANDROID_NDK} )
    if( EXISTS ${ANDROID_NDK_DEFAULT_SEARCH_PATH} )
        set ( ANDROID_NDK ${ANDROID_NDK_DEFAULT_SEARCH_PATH} )
        message( STATUS "Using default path for android NDK ${ANDROID_NDK}" )
        message( STATUS "If you prefer to use a different location, please define the environment variable: ANDROID_NDK" )
    endif()
endif()

#try to find toolchain
if( NOT DEFINED ANDROID_NDK_TOOLCHAIN_ROOT )
    set( ANDROID_NDK_TOOLCHAIN_ROOT $ENV{ANDROID_NDK_TOOLCHAIN_ROOT} )
endif()

if( NOT EXISTS ${ANDROID_NDK_TOOLCHAIN_ROOT} )
    set( ANDROID_NDK_TOOLCHAIN_ROOT ${ANDROID_NDK_TOOLCHAIN_DEFAULT_SEARCH_PATH} )
    message( STATUS "Using default path for toolchain ${ANDROID_NDK_TOOLCHAIN_ROOT}" )
    message( STATUS "If you prefer to use a different location, please define the environment variable: ANDROID_NDK_TOOLCHAIN_ROOT" )
endif()

# setup environment
if( NOT DEFINED BUILD_WITH_ANDROID_NDK AND NOT DEFINED BUILD_WITH_ANDROID_NDK_TOOLCHAIN )
    if( EXISTS ${ANDROID_NDK_TOOLCHAIN_ROOT} )
        message( STATUS "Using android NDK standalone toolchain from ${ANDROID_NDK_TOOLCHAIN_ROOT}" )
        set( ANDROID_NDK_TOOLCHAIN_ROOT ${ANDROID_NDK_TOOLCHAIN_ROOT} CACHE PATH "root of the Android NDK standalone toolchain" FORCE )
        set( ANDROID_NDK_SYSROOT "${ANDROID_NDK_TOOLCHAIN_ROOT}/sysroot" )
        set( BUILD_WITH_ANDROID_NDK_TOOLCHAIN True )
    elseif( EXISTS ${ANDROID_NDK} )
        set( ANDROID_NDK ${ANDROID_NDK} CACHE PATH "root of the android ndk" FORCE )

        # figure out ANDROID_LEVEL
        set( ANDROID_LEVEL $ENV{ANDROID_LEVEL} )
        string( REGEX REPLACE "android-([0-9]+)" "\\1" ANDROID_LEVEL "${ANDROID_LEVEL}" )

        set( PossibleAndroidLevels "3;4;5;8;9" )
        set( ANDROID_LEVEL ${ANDROID_LEVEL} CACHE STRING "android API level" )
        set_property( CACHE ANDROID_LEVEL PROPERTY STRINGS ${PossibleAndroidLevels} )

        if( NOT ANDROID_LEVEL GREATER 2 )
            set( ANDROID_LEVEL 9 CACHE STRING "android API level" FORCE )
            message( STATUS "Using default android API level android-${ANDROID_LEVEL}" )
            message( STATUS "If you prefer to use a different API level, please define the environment variable: ANDROID_LEVEL" )
        endif()

        set( ANDROID_NDK_TOOLCHAIN_ROOT "${ANDROID_NDK}/toolchains/arm-linux-androideabi-4.4.3/prebuilt/${NDKSYSTEM}" )
        set( ANDROID_NDK_SYSROOT "${ANDROID_NDK}/platforms/android-${ANDROID_LEVEL}/arch-arm/" )

        message( STATUS "Using android NDK from ${ANDROID_NDK}" )
        set( BUILD_WITH_ANDROID_NDK True )
    else()
        message( FATAL_ERROR "neither ${ANDROID_NDK} nor ${ANDROID_NDK_TOOLCHAIN_ROOT} does not exist!
        You should either set an environment variable:
          export ANDROID_NDK=~/my-android-ndk
        or
          export ANDROID_NDK_TOOLCHAIN_ROOT=~/my-android-toolchain
        or put the toolchain or NDK in the default path:
          sudo ln -s ~/my-android-ndk ${ANDROID_NDK_DEFAULT_SEARCH_PATH}
          sudo ln -s ~/my-android-toolchain ${ANDROID_NDK_TOOLCHAIN_DEFAULT_SEARCH_PATH}" )
    endif()
endif()

# specify the cross compiler
set( CMAKE_C_COMPILER   ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-gcc${TOOL_OS_SUFFIX}     CACHE PATH "gcc" FORCE )
set( CMAKE_CXX_COMPILER ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-g++${TOOL_OS_SUFFIX}     CACHE PATH "g++" FORCE )
#there may be a way to make cmake deduce these TODO deduce the rest of the tools
set( CMAKE_AR           ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-ar${TOOL_OS_SUFFIX}      CACHE PATH "archive" FORCE )
set( CMAKE_LINKER       ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-ld${TOOL_OS_SUFFIX}      CACHE PATH "linker" FORCE )
set( CMAKE_NM           ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-nm${TOOL_OS_SUFFIX}      CACHE PATH "nm" FORCE )
set( CMAKE_OBJCOPY      ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-objcopy${TOOL_OS_SUFFIX} CACHE PATH "objcopy" FORCE )
set( CMAKE_OBJDUMP      ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-objdump${TOOL_OS_SUFFIX} CACHE PATH "objdump" FORCE )
set( CMAKE_STRIP        ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-strip${TOOL_OS_SUFFIX}   CACHE PATH "strip" FORCE )
set( CMAKE_RANLIB       ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin/arm-linux-androideabi-ranlib${TOOL_OS_SUFFIX}  CACHE PATH "ranlib" FORCE )

#setup build targets, mutually exclusive
set( PossibleArmTargets "armeabi;armeabi-v7a;armeabi-v7a with NEON;armeabi-v7a with VFPV3" )
set( ARM_TARGET "armeabi-v7a" CACHE STRING "the arm target for android, recommend armeabi-v7a for floating point support and NEON." )
set_property( CACHE ARM_TARGET PROPERTY STRINGS ${PossibleArmTargets} )

#set these flags for client use
if( ARM_TARGET STREQUAL "armeabi" )
    set( ARMEABI true )
    set( ARMEABI_NDK_NAME "armeabi" )
    set( NEON false )
else()
    if( ARM_TARGET STREQUAL "armeabi-v7a with NEON" )
        set( NEON true )
        set( VFPV3 true )
    elseif( ARM_TARGET STREQUAL "armeabi-v7a with VFPV3" )
        set( VFPV3 true )
    endif()
    set( ARMEABI_V7A true )
    set( ARMEABI_NDK_NAME "armeabi-v7a" )
endif()

# where is the target environment 
if( BUILD_WITH_ANDROID_NDK )
    set( CMAKE_FIND_ROOT_PATH  ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin ${ANDROID_NDK_TOOLCHAIN_ROOT}/arm-linux-androideabi ${ANDROID_NDK_SYSROOT} )
    include_directories( ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/include ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/libs/${ARMEABI_NDK_NAME}/include )
endif()

if( BUILD_WITH_ANDROID_NDK_TOOLCHAIN )
    set( CMAKE_FIND_ROOT_PATH  ${ANDROID_NDK_TOOLCHAIN_ROOT}/bin ${ANDROID_NDK_TOOLCHAIN_ROOT}/arm-linux-androideabi ${ANDROID_NDK_TOOLCHAIN_ROOT}/sysroot ${CMAKE_INSTALL_PREFIX} ${CMAKE_INSTALL_PREFIX}/share )
    #for some reason this is needed? TODO figure out why...
    include_directories( ${ANDROID_NDK_TOOLCHAIN_ROOT}/arm-linux-androideabi/include/c++/4.4.3/arm-linux-androideabi )
endif()

# allow programs like swig to be found -- but can be deceiving for
# system tool dependencies.
set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH )
# only search for libraries and includes in the ndk toolchain
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

set( CMAKE_CXX_FLAGS "-fPIC -DANDROID -Wno-psabi -fsigned-char" )
set( CMAKE_C_FLAGS "-fPIC -DANDROID -Wno-psabi -fsigned-char" )

set( FORCE_ARM OFF CACHE BOOL "Use 32-bit ARM instructions instead of Thumb-1" )
if( NOT FORCE_ARM )
    #It is recommended to use the -mthumb compiler flag to force the generation
    #of 16-bit Thumb-1 instructions (the default being 32-bit ARM ones).
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthumb" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb" )
else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -marm" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -marm" )
endif()

set( CMAKE_CXX_FLAGS "--sysroot=${ANDROID_NDK_SYSROOT} ${CMAKE_CXX_FLAGS}" )
set( CMAKE_C_FLAGS "--sysroot=${ANDROID_NDK_SYSROOT} ${CMAKE_C_FLAGS}" )

if( ARMEABI_V7A )
 #these are required flags for android armv7-a
 set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mfloat-abi=softfp" )
 set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7-a -mfloat-abi=softfp" )
 if( NEON )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon" )
 elseif( VFPV3 )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfpv3" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfpv3" )
 endif()
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "c flags" )

set( STL_LIBRARIES_PATH "${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/libs/${ARMEABI_NDK_NAME}" )
      
#Also, this is *required* to use the following linker flags that routes around
#a CPU bug in some Cortex-A8 implementations:
set( LINKER_FLAGS "-Wl,--fix-cortex-a8 -L${STL_LIBRARIES_PATH} -lstdc++ -lsupc++ " )

set( NO_UNDEFINED ON CACHE BOOL "Don't all undefined symbols" )
if( NO_UNDEFINED )
 set( LINKER_FLAGS "-Wl,--no-undefined ${LINKER_FLAGS}" )
endif()

set( CMAKE_SHARED_LINKER_FLAGS "${LINKER_FLAGS}" CACHE STRING "linker flags" FORCE )
set( CMAKE_MODULE_LINKER_FLAGS "${LINKER_FLAGS}" CACHE STRING "linker flags" FORCE )
set( CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS}" CACHE STRING "linker flags" FORCE )

#set this global flag for cmake client scripts to change behavior
set( ANDROID True )

