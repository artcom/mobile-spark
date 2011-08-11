add_subdirectory(Tests)

set(SOURCES
APK_functions.cpp
)

set(HEADERS
APK_functions.h
)

add_library(android STATIC ${SOURCES} ${HEADERS})

include_directories (${Boost_INCLUDE_DIRS})
include_directories(${LIBZIP_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core/src/base)

target_link_libraries(android zip)
