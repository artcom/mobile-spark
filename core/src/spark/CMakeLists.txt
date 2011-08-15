
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(spark SHARED ${SOURCES} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${LIBZIP_INCLUDE_DIR})
include_directories(${CML_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/core/src)

target_link_libraries(spark mar animation android)

