add_subdirectory(Tests)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(android SHARED ${SOURCES} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBZIP_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core/src)

target_link_libraries(android masl zip)
