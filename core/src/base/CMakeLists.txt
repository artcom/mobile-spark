add_subdirectory(Tests)

set(SOURCES
BaseApp.cpp
BaseEntry.cpp
Exception.cpp
Logger.cpp
MatrixStack.cpp
string_functions.cpp
UnitTest.cpp
XMLNode.cpp
XMLUtils.cpp
)

set(HEADERS
BaseApp.h
BaseEntry.h
Exception.h
Logger.h
MatrixStack.h
openGL_functions.h
Singleton.h
string_functions.h
UnitTest.h
XMLNode.h
XMLUtils.h
)

include_directories (${LIBXML2_INCLUDE_DIRS})
add_library(base SHARED ${SOURCES} ${HEADERS})
find_library(ANDROID_LOG_LIB log )
find_library(OPENGL_ES_20 GLESv2)

find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp HINTS ${PROJECT_SOURCE_DIR}/core/netsrc $ENV{BOOST_HOME} /opt/local/include NO_CMAKE_FIND_ROOT_PATH)
include_directories (${Boost_INCLUDE_DIRS})

find_path(CML_INCLUDE_DIR cml/cml.h ${PROJECT_SOURCE_DIR}/core/netsrc NO_CMAKE_FIND_ROOT_PATH)
include_directories (${CML_INCLUDE_DIR})

#only for android??
#@cmake-experts: shouldn't it be enough to link against android that uses zip?
find_path(LIBZIP_INCLUDE_DIR zip.h ${PROJECT_SOURCE_DIR}/core/netsrc/libzip/ NO_CMAKE_FIND_ROOT_PATH)
include_directories(${LIBZIP_INCLUDE_DIR})

 # add imported target
add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET LIBXML2 PROPERTY
            IMPORTED_LOCATION ${LIBXML2_LIBRARY})
target_link_libraries(base android LIBXML2 ${ANDROID_LOG_LIB} ${OPENGL_ES_20})
