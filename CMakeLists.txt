cmake_minimum_required(VERSION 2.8)

project(mobile-spark)
list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

add_subdirectory(acmake)
find_package(AcMake)
add_subdirectory(core)
