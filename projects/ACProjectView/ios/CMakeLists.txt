# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
#
# It is distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

file(GLOB SOURCES *.m *.mm)
file(GLOB HEADERS *.h *.pch)

IF(${TARGET_PLATFORM} STREQUAL iPhoneOS)
    set(CMAKE_EXE_LINKER_FLAGS "-arch armv7 -miphoneos-version-min=${SDKVER}")
ELSEIF(${TARGET_PLATFORM} STREQUAL iPhoneSimulator)
    set(CMAKE_EXE_LINKER_FLAGS "-arch i386 -miphoneos-version-min=${SDKVER}")
ENDIF()

# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
#set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "artcom.${APP_NAME}")
set(MACOSX_BUNDLE_ICON_FILE "ACProjectView/textures/ios_icon.png")
#set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VTK_VERSION}")
#set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
#set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION})
#set(MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION})
#set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved.")

#set include paths
include_directories(../../../iOS)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${libXML2_INCLUDE_DIRS})
include_directories(${Cml_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/core/src)  
include_directories(../src)

set(EXECUTABLE_NAME ACProjectViewApp)
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
ADD_FRAMEWORK(${EXECUTABLE_NAME} UIKit)
ADD_FRAMEWORK(${EXECUTABLE_NAME} Foundation)
ADD_FRAMEWORK(${EXECUTABLE_NAME} QuartzCore)
ADD_FRAMEWORK(${EXECUTABLE_NAME} OpenGLES)
ADD_FRAMEWORK(${EXECUTABLE_NAME} CoreMotion)
ADD_FRAMEWORK(${EXECUTABLE_NAME} CoreGraphics)
ADD_FRAMEWORK(${EXECUTABLE_NAME} CoreMedia)
ADD_FRAMEWORK(${EXECUTABLE_NAME} CoreVideo)
ADD_FRAMEWORK(${EXECUTABLE_NAME} CoreText)
ADD_FRAMEWORK(${EXECUTABLE_NAME} AVFoundation)
ADD_FRAMEWORK(${EXECUTABLE_NAME} OpenAL)
ADD_FRAMEWORK(${EXECUTABLE_NAME} AudioToolbox)

#Link libraries
target_link_libraries(${EXECUTABLE_NAME} ACProjectView)
target_link_libraries(${EXECUTABLE_NAME} ${masl_LIBRARY_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${mar_LIBRARY_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${animation_LIBRARY_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${ios_LIBRARY_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${spark_LIBRARY_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${SparkViewerBase_LIBRARY_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${png_LIBRARY_DIRS})
find_library(ZLIB_LIBRARY_DIRS
   NAMES z zlib
)
find_library(LIBGCC_LIBRARY_DIRS 
   NAMES libgcc_eh.a
   HINTS "${SDKROOT}/usr/lib/gcc/arm-apple-darwin10/4.2.1"
)
target_link_libraries(${EXECUTABLE_NAME} ${ZLIB_LIBRARY_DIRS})

if(LIBGCC_LIBRARY_DIRS)
    target_link_libraries(${EXECUTABLE_NAME} ${LIBGCC_LIBRARY_DIRS})
endif(LIBGCC_LIBRARY_DIRS)

#This creates folders in the *.app with the mkdir command
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND cd ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app && mkdir -p ${APP_NAME} && cd -
)

#This copies the asset folder into the iOS app, 
#as well as if you drag and drop the folder into the Xcodeproject 
#or into the final app by opening it with the packetmanager.
#copy assets folder
#set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/android/${APP_NAME}/assets)
#add_custom_command( 
#    TARGET ${EXECUTABLE_NAME} 
#    POST_BUILD 
#    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/#${EXECUTABLE_NAME}.app/
#)

#copy layouts folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/layouts)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy models folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/models)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy shaders folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/shaders)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy textures folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/textures)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy assets layouts folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets/layouts)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR}/* ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/layouts/
)

#copy assets texturesfolder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets/textures)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR}/* ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/textures/
)



#copy fonts folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/fonts)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy sounds folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/sounds)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/
)

#copy core/shaders folder
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/../../core/shaders)
add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -resolve-src-symlinks ${ASSETS_DIR}/* ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/${APP_NAME}/shaders/
)

add_custom_command( 
    TARGET ${EXECUTABLE_NAME} 
    POST_BUILD 
    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp ${PROJECT_SOURCE_DIR}/../../iOS/Environment.plist  ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.app/
)
