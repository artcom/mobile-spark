file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(EXTERNS Boost Cml libXML2)
set(DEPENDS animation mar)
if(ANDROID)
    list(APPEND DEPENDS android)
else(ANDROID)
    list(APPEND DEPENDS iOS)
endif(ANDROID)

ac_add_library(
    spark spark
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS ${DEPENDS}
    SHARED
)
