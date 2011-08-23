set(ENV{ARM_TARGET} armeabi)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/acmake/toolchains)
include(android.toolchain)
