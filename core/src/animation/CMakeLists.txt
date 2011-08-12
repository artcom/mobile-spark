file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(animation SHARED ${SOURCES} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(animation)
