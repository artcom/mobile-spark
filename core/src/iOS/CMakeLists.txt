file(GLOB SOURCES *.cpp *.mm *.m)
file(GLOB HEADERS *.h)

set(EXTERNS Boost)

ac_add_library(
    iOS iOS
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    EXTERNS ${EXTERNS}
    DEPENDS masl mar png
)

ADD_FRAMEWORK(iOS CoreGraphics)
ADD_FRAMEWORK(iOS QuartzCore)
ADD_FRAMEWORK(iOS CoreText)
ADD_FRAMEWORK(iOS CoreMedia)
ADD_FRAMEWORK(iOS AVFoundation)
