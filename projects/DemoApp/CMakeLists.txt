cmake_minimum_required(VERSION 2.8)

set(APP_NAME DemoApp)
project(${APP_NAME})

list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

set(core_SOURCE_DIR ../../core)

add_subdirectory(../../acmake "${CMAKE_CURRENT_BINARY_DIR}/acmake")
find_package(AcMake)

set(spark_DIR ../../core/src/spark)
find_package(spark)

set(REQUIRED_PACKAGES Boost Cml libXML2)
foreach(PACKAGE ${REQUIRED_PACKAGES})
    find_package(${PACKAGE})
endforeach()


include_directories(../../core/src)
add_subdirectory(src)
if(IOS)
    add_subdirectory(ios)
endif()
