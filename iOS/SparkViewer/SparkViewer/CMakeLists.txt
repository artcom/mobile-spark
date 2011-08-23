file(GLOB SOURCES *.m *.mm)
file(GLOB HEADERS *.h *.pch)

IF(${TARGET_PLATFORM} STREQUAL iPhoneOS)
    set(CMAKE_EXE_LINKER_FLAGS "-arch armv7 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER}")
ELSEIF(${TARGET_PLATFORM} STREQUAL iPhoneSimulator)
    set(CMAKE_EXE_LINKER_FLAGS "-arch i386 --isysroot=${SDKROOT} -miphoneos-version-min=${SDKVER}")
ENDIF()

# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
#set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "artcom.${APP_NAME}")
#set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}")
#set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
#set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION})
#set(MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION})
#set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved.")

# These variables are specific to our plist and are NOT standard CMake variables
#set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainWindow")
#set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")

#Builds the *.app
set(APP_TYPE MACOSX_BUNDLE)
add_executable(${APP_NAME}
	${APP_TYPE}
	${HEADERS}
	${SOURCES}
)
	
# Set a custom plist file for the app bundle
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST Info.plist)
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: xxx")

LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/System/Library/Frameworks")

#Apple ios Frameworks
ADD_FRAMEWORK(${APP_NAME} UIKit)
ADD_FRAMEWORK(${APP_NAME} Foundation)
ADD_FRAMEWORK(${APP_NAME} QuartzCore)
ADD_FRAMEWORK(${APP_NAME} OpenGLES)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${CML_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core/src)
	
target_link_libraries(${APP_NAME} spark)

#This copies the asset folder into the iOS app, 
#as well as if you drag and drop the folder into the Xcodeproject 
#or into the final app by opening it with the packetmanager.
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(RES_DIR ${PROJECT_SOURCE_DIR}/android/SparkViewer/assets)
add_custom_command( 
  	TARGET ${APP_NAME} 
	POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${RES_DIR} ${EXECUTABLE_OUTPUT_PATH}/${APP_NAME}.app
)
	

	###### InterFaceBuilder not tested yet

	# Make sure we can find the 'ibtool' program. If we can NOT find it we
	# skip generation of this project
	#find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin" "/Developer/usr/bin")
	#if (${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
  	#	message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
	#endif()

	# Make sure the 'Resources' Directory is correctly created before we build
	#add_custom_command (TARGET ${APP_NAME} PRE_BUILD COMMAND mkdir -p ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/${APP_NAME}.app/Contents/Resources)

	# Compile the .xib files using the 'ibtool' program with the destination being the app package
	#foreach(xib ${XIB_FILES})
  	#	add_custom_command (TARGET ${APP_NAME} POST_BUILD 
    #                  COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text 
    #                          --compile ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/${APP_NAME}.app/Contents/Resources/${xib}.nib 
    #                         ${SimpleCocoaVTK_SOURCE_DIR}/${xib}.xib
    #                  COMMENT "Compiling ${SimpleCocoaVTK_SOURCE_DIR}/${xib}.xib")

	#endforeach()