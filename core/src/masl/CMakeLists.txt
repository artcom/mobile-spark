
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(masl SHARED ${SOURCES} ${HEADERS})

find_library(ANDROID_LOG_LIB log )
find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp HINTS ${PROJECT_SOURCE_DIR}/core/netsrc $ENV{BOOST_HOME} /opt/local/include NO_CMAKE_FIND_ROOT_PATH)
find_path(CML_INCLUDE_DIR cml/cml.h ${PROJECT_SOURCE_DIR}/core/netsrc NO_CMAKE_FIND_ROOT_PATH)
#only for android??
find_path(LIBZIP_INCLUDE_DIR libzip/zip.h ${PROJECT_SOURCE_DIR}/core/netsrc/ NO_CMAKE_FIND_ROOT_PATH)

include_directories(${LIBZIP_INCLUDE_DIR})
include_directories (${LIBXML2_INCLUDE_DIRS})
include_directories (${CML_INCLUDE_DIR})
include_directories (${Boost_INCLUDE_DIRS})

 # add imported target
add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET LIBXML2 PROPERTY
            IMPORTED_LOCATION ${LIBXML2_LIBRARY})
target_link_libraries(masl LIBXML2 ${ANDROID_LOG_LIB} )

add_subdirectory(Tests)
