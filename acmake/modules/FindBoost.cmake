# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp HINTS ${core_SOURCE_DIR}/netsrc/boost/include $ENV{MOBILESPARK_BOOST_HOME} NO_CMAKE_FIND_ROOT_PATH)

if(ANDROID)
    find_path(Boost_LIBRARY_DIR libboost_chrono.a HINTS ${core_SOURCE_DIR}/netsrc/boost/android/ $ENV{MOBILESPARK_BOOST_HOME} NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
else(ANDROID)
    find_path(Boost_LIBRARY_DIR libboost_chrono.a HINTS ${core_SOURCE_DIR}/netsrc/boost/ios/ $ENV{MOBILESPARK_BOOST_HOME} NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
endif()


if(ANDROID)
else(ANDROID)
    list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_date_time.a)
    list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_graph.a)
endif()

list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_exception.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_random.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_regex.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_wave.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_chrono.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_iostreams.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_signals.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_filesystem.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_system.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_thread.a)
list(APPEND Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIR}/libboost_timer.a)


