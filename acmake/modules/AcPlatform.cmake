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

if(ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${ARMEABI_NDK_NAME}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ARMEABI_NDK_NAME}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${ARMEABI_NDK_NAME}")

    add_definitions(-DANDROID)
else(ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

    add_definitions(-DiOS)
endif(ANDROID)
