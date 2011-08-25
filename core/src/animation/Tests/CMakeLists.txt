set(SOURCES
TestAnimation.cpp
)

set(HEADERS
TestAnimation.h
)

set(EXTERNS Boost)

if(ANDROID)
    ac_add_library(
        testAnimation testAnimation
        SOURCES ${SOURCES}
        HEADERS ${HEADERS}
        EXTERNS ${EXTERNS}
        DEPENDS animation masl 
        SHARED
    )
else(ANDROID)
    ac_add_library(
        testAnimation testAnimation
        SOURCES ${SOURCES}
        HEADERS ${HEADERS}
        EXTERNS ${EXTERNS}
        DEPENDS animation masl 
    )
endif(ANDROID)
