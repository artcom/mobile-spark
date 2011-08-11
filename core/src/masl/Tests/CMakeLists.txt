set(SOURCES
TestMasl.cpp
)

set(HEADERS
TestMasl.h
)

add_library(testMasl SHARED ${SOURCES} ${HEADERS})

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${LIBXML2_INCLUDE_DIRS})
include_directories (${CML_INCLUDE_DIR})

target_link_libraries(testMasl masl)
