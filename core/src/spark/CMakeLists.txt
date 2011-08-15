
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(spark SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(spark STATIC ${SOURCES} ${HEADERS})
endif()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${CML_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/core/src)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	include_directories(${LIBZIP_INCLUDE_DIR})
	target_link_libraries(spark mar android)
	
elseif($ENV{IS_TARGET_PLATFORM_IOS})

	target_link_libraries(spark mar)
	
	IF(${TARGET_PLATTFORM} STREQUAL iPhoneOS)
		set(CMAKE_EXE_LINKER_FLAGS "-arch armv7 --isysroot=${SDKROOT} -miphoneos-version-min=$	{SDKVER} -L${SDKROOT}/usr/lib -L${SDKROOT}/usr/lib/system")
	ELSEIF(${TARGET_PLATTFORM} STREQUAL iPhoneSimulator)
		set(CMAKE_EXE_LINKER_FLAGS "-arch i686 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER} -L${SDKROOT}/usr/lib -L${SDKROOT}/usr/lib/system")
	ENDIF()

	set(MACOSX_BUNDLE_GUI_IDENTIFIER "artcom.spark_viewer")
	set(APP_TYPE MACOSX_BUNDLE)

	add_executable(${APP_NAME}
		${APP_TYPE}
		${HEADERS}
		${SOURCES}
	)

	# code signing
	#set_target_properties(${APP_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: Arvid Hansen DiÌ%C2%82å%C2%ADaz (8ZR2XFRZ48)")

	ADD_FRAMEWORK(${APP_NAME} UIKit)
	ADD_FRAMEWORK(${APP_NAME} Foundation)
	target_link_libraries(${APP_NAME} spark)
	
endif()