set(SOURCES
TestSpark.cpp
)

set(HEADERS
TestSpark.h
)

set(EXTERNS Boost)

ac_add_library(
    testSpark testSpark
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl spark
)
