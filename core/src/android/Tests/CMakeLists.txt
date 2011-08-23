set(SOURCES
    TestAndroid.cpp
)

set(HEADERS
    TestAndroid.h
)

set(EXTERNS Boost)

ac_add_library(
    android android
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl android zip
)
