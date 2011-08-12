
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(mar SHARED ${SOURCES} ${HEADERS})

find_library(OPENGL_ES_20 GLESv2)

include_directories(${PROJECT_SOURCE_DIR}/core/src)
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${CML_INCLUDE_DIR})
include_directories (${LIBXML2_INCLUDE_DIRS})

target_link_libraries(mar masl ${OPENGL_ES_20})
