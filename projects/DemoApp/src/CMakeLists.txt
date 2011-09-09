file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(DEPENDS spark)

ac_add_library(
    ${APP_NAME} ${APP_NAME} 
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS ${DEPENDS}
)

