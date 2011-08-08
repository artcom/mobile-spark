set(SOURCES
Logger.cpp
string_functions.cpp
Exception.cpp
CppEntry.cpp
UnitTest.cpp
XMLNode.cpp
XMLUtils.cpp
)

set(HEADERS
Singleton.h
Exception.h
Logger.h
string_functions.h
UnitTest.h
CppEntry.h
XMLNode.h
XMLUtils.h
)

include_directories (${LIBXML2_INCLUDE_DIRS})
add_library(base SHARED ${SOURCES} ${HEADERS})
find_library(ANDROID_LOG_LIB log )

find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp NO_CMAKE_FIND_ROOT_PATH)
include_directories (${Boost_INCLUDE_DIRS})

 # add imported target
add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET LIBXML2 PROPERTY
            IMPORTED_LOCATION ${LIBXML2_LIBRARY})
target_link_libraries(base LIBXML2 ${ANDROID_LOG_LIB})
