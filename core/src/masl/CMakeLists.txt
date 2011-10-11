file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(EXTERNS Boost Cml libXML2)
if(ANDROID)
    #list(APPEND EXTERNS libCurl)
    find_library(log_LIBRARY_DIRS log )
    list(APPEND EXTERNS log)
endif(ANDROID)

ac_add_library(
    masl masl
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
)

add_subdirectory(Tests)
