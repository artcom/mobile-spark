file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(animation SHARED ${SOURCES} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/core/src)

target_link_libraries(animation masl)
