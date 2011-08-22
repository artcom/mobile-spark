cmake_minimum_required(VERSION 2.8)

set(APP_NAME mobile-spark)
project(${APP_NAME})

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

add_subdirectory(acmake)
find_package(AcMake)

add_subdirectory(core)

if(ANDROID)
    #test for ANDROID and look for stuff not relevant to android.
    #if(NOT ANDROID)
    #   #jni is available by default on android
    #   find_package(JNI REQUIRED)
    #   include_directories(${JNI_INCLUDE_DIRS})
    #endif()
elseif(IOS)
    add_subdirectory(iOS)
endif()

