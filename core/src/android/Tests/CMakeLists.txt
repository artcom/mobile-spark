set(SOURCES
    TestAndroid.cpp
)

set(HEADERS
    TestAndroid.h
)

add_library(testAndroid SHARED ${SOURCES} ${HEADERS})
include_directories (${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/core/src/base)
include_directories(${PROJECT_SOURCE_DIR}/core/netsrc/libzip)

target_link_libraries(testAndroid android base zip)
