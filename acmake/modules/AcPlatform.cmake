# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2009, ART+COM AG Berlin, Germany <www.artcom.de>
#
# This file is part of the ART+COM CMake Library (acmake).
#
# It is distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)             
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
#
# This file declares cmake- and c-level discrimination
# variables for each platform supported by acmake.
#
# It also cares about various C low-level spells required
# for what we consider a sane C runtime environment:
#
#  - libc should be reentrant / threadsafe
#  - libm should be available and linked
#  - pthreads should be available (XXX: need pthread-win32 on windows)
#  - posix realtime library should be available
#  - dlopen should be available (XXX: not on windows)
#
# XXX: This file might need some re-thinking. Works for now though.
#
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


##test for ANDROID and look for stuff not relevant to android.
#if(NOT ANDROID)
##jni is available by default on android
#find_package(JNI REQUIRED)
#include_directories(${JNI_INCLUDE_DIRS})
#endif()
if(ANDROID)
    add_definitions(-DANDROID)
else(ANDROID)
    add_definitions(-DiOS)
endif(ANDROID)

