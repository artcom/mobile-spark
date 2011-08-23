file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

set(EXTERNS Boost Cml libXML2)
if(ANDROID)
	find_library(GLES20_LIBRARY_DIRS GLESv2)
    list(APPEND DEPENDS android)
    list(APPEND EXTERNS GLES20)
endif(ANDROID)

ac_add_library(
    mar mar
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl
)
if(IOS)
	ADD_FRAMEWORK(mar OpenGLES)
endif(IOS)


