file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(EXTERNS Boost libXML2 Cml masl mar animation spark)
if (ANDROID) 
    list(APPEND EXTERNS android)
else (ANDROID)
    list(APPEND EXTERNS ios)
endif(ANDROID)

ac_add_library(
    ${APP_NAME} ${APP_NAME} 
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS ${DEPENDS}
    EXTERNS ${EXTERNS}
)

