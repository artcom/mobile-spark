cmake_minimum_required(VERSION 2.8)
#SET(TARGET_PLATTFORM iPhoneOS)
SET(TARGET_PLATTFORM iPhoneSimulator)

message("in CMakeLists")
# SDK Info
SET (SDKVER "4.3")
SET (DEVROOT "/Developer/Platforms/${TARGET_PLATTFORM}.platform/Developer")
SET (SDKROOT "${DEVROOT}/SDKs/${TARGET_PLATTFORM}${SDKVER}.sdk")
SET (CMAKE_OSX_SYSROOT "${SDKROOT}")

set(NAME test)
project(${NAME})

file(GLOB headers *.h)
file(GLOB sources *.m)

message("in source:  ${SDKVER}")

IF(${TARGET_PLATTFORM} STREQUAL iPhoneOS)
	set(CMAKE_EXE_LINKER_FLAGS "-arch armv7 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER} -L${SDKROOT}/usr/lib -L${SDKROOT}/usr/lib/system")
ELSEIF(${TARGET_PLATTFORM} STREQUAL iPhoneSimulator)
	set(CMAKE_EXE_LINKER_FLAGS "-arch i686 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER} -L${SDKROOT}/usr/lib -L${SDKROOT}/usr/lib/system")
ENDIF()

set(MACOSX_BUNDLE_GUI_IDENTIFIER "arvid.test")
set(APP_TYPE MACOSX_BUNDLE)

add_executable(${NAME}
	${APP_TYPE}
	${headers}
	${sources}
	)

# code signing
#set_target_properties(${NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: Arvid Hansen DiÌ%C2%82å%C2%ADaz (8ZR2XFRZ48)")


MACRO(ADD_FRAMEWORK appname fwname)
	find_library(FRAMEWORK_${fwname}
		NAMES ${fwname}
		PATHS ${SDKROOT}/System/Library
		PATH_SUFFIXES Frameworks
		NO_DEFAULT_PATH)
	if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
		MESSAGE(ERROR ": Framework ${fwname} not found")
	else()
		TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
		MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
	endif()
endmacro(ADD_FRAMEWORK)

ADD_FRAMEWORK(${NAME} UIKit)
ADD_FRAMEWORK(${NAME} Foundation)


