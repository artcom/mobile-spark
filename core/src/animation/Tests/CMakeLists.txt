set(SOURCES
TestAnimation.cpp
)

set(HEADERS
TestAnimation.h
)

set(EXTERNS Boost)

ac_add_library(
    testAnimation testAnimation
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS animation masl 
    SHARED
)
