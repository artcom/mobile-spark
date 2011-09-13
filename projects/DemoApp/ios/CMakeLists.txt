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

set(EXECUTABLE_NAME demo)

#Builds the *.app
set(APP_TYPE MACOSX_BUNDLE)
add_executable(${EXECUTABLE_NAME}
    ${APP_TYPE}
    ${HEADERS}
    ${SOURCES}
)
 
#Release and Debug Build path
SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/")
SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/")
  
# Set a custom plist file for the app bundle
#SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST Info.plist)
#SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: xxx")

LINK_DIRECTORIES("${SDKROOT}/usr/lib")
LINK_DIRECTORIES("${SDKROOT}/System/Library/Frameworks")

#Apple ios Frameworks
#ADD_FRAMEWORK(${EXECUTABLE_NAME} UIKit)
   
target_link_libraries(${EXECUTABLE_NAME} SparkViewerBase)

#This copies the asset folder into the iOS app, 
#as well as if you drag and drop the folder into the Xcodeproject 
#or into the final app by opening it with the packetmanager.

#copy assets folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/layouts)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy assets folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/models)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy assets folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/shaders)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy assets folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/textures)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy assets folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/../../core/shaders)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/core/
)

   