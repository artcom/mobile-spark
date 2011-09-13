file(GLOB SOURCES *.cpp *.mm *.m)
file(GLOB HEADERS *.h)

set(EXTERNS Boost)

ac_add_library(
    iOS iOS
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl 
)

