file(GLOB SOURCES *.c)
set (HEADERS
zip.h
)

find_library(ZLIB_LIBRARY_DIRS
   NAMES z zlib
)

ac_add_library(zip zip 
    SOURCES ${SOURCES} 
    HEADERS ${HEADERS}
    EXTERNS ZLIB
    STATIC )

