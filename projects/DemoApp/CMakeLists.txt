cmake_minimum_required(VERSION 2.8)

set(APP_NAME demoapp)
project(${APP_NAME})

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

find_package(spark)

set(REQUIRED_PACKAGES Boost Cml)
foreach(PACKAGE ${REQUIRED_PACKAGES})
    find_package(${PACKAGE})
endforeach()

add_subdirectory(src)

