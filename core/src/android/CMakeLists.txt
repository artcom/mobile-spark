add_subdirectory(Tests)

set(SOURCES
APK_functions.cpp
)

set(HEADERS
APK_functions.h
)

add_library(android STATIC ${SOURCES} ${HEADERS})

find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp HINTS ${PROJECT_SOURCE_DIR}/core/netsrc $ENV{BOOST_HOME} /opt/local/include NO_CMAKE_FIND_ROOT_PATH)
include_directories (${Boost_INCLUDE_DIRS})

find_path(LIBZIP_INCLUDE_DIR zip.h ${PROJECT_SOURCE_DIR}/core/netsrc/libzip/ NO_CMAKE_FIND_ROOT_PATH)
include_directories(${LIBZIP_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core/src/base)

target_link_libraries(android zip)
