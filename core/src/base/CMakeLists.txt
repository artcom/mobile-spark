set(SOURCES
CppEntry.cpp
XMLNode.cpp
XMLUtils.cpp
)

set(HEADERS
CppEntry.h
XMLNode.h
XMLUtils.h
log.h
)

include_directories (${LIBXML2_INCLUDE_DIRS})
add_library(base SHARED ${SOURCES} ${HEADERS})
find_library(ANDROID_LOG_LIB log )
 # add imported target
add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET LIBXML2 PROPERTY
            IMPORTED_LOCATION ${LIBXML2_LIBRARY})
target_link_libraries(base LIBXML2 ${ANDROID_LOG_LIB})
