set(SOURCES
TestBase.cpp
)

set(HEADERS
TestBase.h
)

include_directories (${LIBXML2_INCLUDE_DIRS})
add_library(testBase SHARED ${SOURCES} ${HEADERS})

include_directories (${Boost_INCLUDE_DIRS})

include_directories (${CML_INCLUDE_DIR})

target_link_libraries(testBase base)
