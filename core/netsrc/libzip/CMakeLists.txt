file(GLOB SOURCES *.c)
set (HEADERS
zip.h
)

find_library(ZLIB_LIBRARY
   NAMES z zlib
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/..)
add_library(zip STATIC ${SOURCES} ${HEADERS})
target_link_libraries(zip ${ZLIB_LIBRARY})

