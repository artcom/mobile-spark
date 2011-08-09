set(SOURCES
CppEntry.cpp
Exception.cpp
Logger.cpp
MatrixStack.cpp
string_functions.cpp
UnitTest.cpp
XMLNode.cpp
XMLUtils.cpp
)

set(HEADERS
CppEntry.h
Exception.h
Logger.h
MatrixStack.h
Singleton.h
string_functions.h
UnitTest.h
XMLNode.h
XMLUtils.h
)

include_directories (${LIBXML2_INCLUDE_DIRS})
add_library(base SHARED ${SOURCES} ${HEADERS})
find_library(ANDROID_LOG_LIB log )

find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp NO_CMAKE_FIND_ROOT_PATH)
include_directories (${Boost_INCLUDE_DIRS})

find_path(CML_INCLUDE_DIR cml/cml.h HINTS core/netsrc/ NO_CMAKE_FIND_ROOT_PATH)
include_directories (${CML_INCLUDE_DIR})

 # add imported target
add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET LIBXML2 PROPERTY
            IMPORTED_LOCATION ${LIBXML2_LIBRARY})
target_link_libraries(base LIBXML2 ${ANDROID_LOG_LIB})
