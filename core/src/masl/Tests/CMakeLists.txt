set(SOURCES
TestMasl.cpp
)

set(HEADERS
TestMasl.h
)

set(EXTERNS Boost Cml libXML2)

ac_add_library(
    testMasl testMasl
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS masl
    EXTERNS ${EXTERNS}
    SHARED
)
