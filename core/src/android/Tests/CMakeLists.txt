set(SOURCES
    TestAndroid.cpp
)

set(HEADERS
    TestAndroid.h
)

set(EXTERNS Boost)

ac_add_library(
    testAndroid testAndroid
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl android zip
    SHARED
)
