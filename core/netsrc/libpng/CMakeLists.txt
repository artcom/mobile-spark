file(GLOB SOURCES *.c)
file(GLOB HEADERS *.h)

#find_library(ZLIB_LIBRARY_DIRS
#   NAMES z zlib
#)

ac_add_library(png png 
    SOURCES ${SOURCES} 
    HEADERS ${HEADERS}
    #EXTERNS ZLIB
    STATIC )

