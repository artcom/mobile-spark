cmake_minimum_required(VERSION 2.8)

<<<<<<< HEAD
project(mobile-spark)
list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_BINARY_DIR})

add_subdirectory(acmake)
find_package(AcMake)
=======
set(APP_NAME mobile-spark)
project(${APP_NAME})

#notice the "recycling" of CMAKE_C_FLAGS
#this is necessary to pick up android flags
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic" )


if($ENV{IS_TARGET_PLATFORM_ANDROID})
	#test for ANDROID and look for stuff not relevant to android.
	if(NOT ANDROID)
		#jni is available by default on android
		find_package(JNI REQUIRED)
		include_directories(${JNI_INCLUDE_DIRS})
	endif()
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	SET(TARGET_PLATFORM $ENV{IOS_PLATFORM})
	#SDK Info
	SET (SDKVER "4.3")
	SET (DEVROOT "/Developer/Platforms/${TARGET_PLATFORM}.platform/Developer")
	SET (SDKROOT "${DEVROOT}/SDKs/${TARGET_PLATFORM}${SDKVER}.sdk")
	SET (CMAKE_OSX_SYSROOT "${SDKROOT}")
	
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
endif()

>>>>>>> 0393e393d5a4ee2c838de224ce39cef07dd2808b
add_subdirectory(core)
add_subdirectory(iOS)
