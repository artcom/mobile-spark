file(GLOB SOURCES *.c)

set (GLOB HEADERS *.h)

find_library(ZLIB_LIBRARY
   NAMES z zlib
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/..)

add_library(png STATIC ${SOURCES} ${HEADERS})

target_link_libraries(png zip ${ZLIB_LIBRARY})

