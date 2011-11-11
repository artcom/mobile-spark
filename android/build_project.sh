#! /bin/bash

./c++build.sh $*
BUILD_OK=$?

VERBOSITY="-quiet"
DEPLOY=0
BUILD_TYPE="debug"

for i in $*
do
    case $i in
        deploy)
            DEPLOY=1
            ;;
        release)
            BUILD_TYPE="release"
            ;;
        verbose)
            VERBOSITY=""
            ;;
        *)
        #unknown
       ;;
   esac
done

ANDROID_TOOL="android"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
fi

APPFOLDER=`pwd`/..
cd $APPFOLDER

# copy assets
if [ $DEPLOY == "1" ]
then
    echo "copy assets"
    FOLDERS="models layouts shaders textures fonts sounds" 
    for folder in $FOLDERS
    do
        echo "copy folder $folder"
        cp -ra $folder android/$PROJECT_NAME/assets
    done

    CORE_FOLDERS="shaders"
    for core_folder in $CORE_FOLDERS
    do
        echo "push core/$core_folder"
        cp -ra ../../core/$core_folder android/$PROJECT_NAME/assets
    done            
fi

# package java
cd android
if [ $BUILD_OK == "0" ] 
then
    cd $PROJECT_NAME
    
    # update Base project
    $ANDROID_TOOL update lib-project --target android-9 --path ../../../../android/SparkViewerBase 

    # update android project
    $ANDROID_TOOL update project --target android-9 --name $PROJECT_NAME --path . 
    $ANDROID_TOOL update project --library ../../../../android/SparkViewerBase --target android-9 --name $PROJECT_NAME --path . 
    BUILD_OK=$?
fi

# in case of deployment, remove test libs
if [ $DEPLOY == "1" ]
then
    pwd
    rm ../../../../_build/lib/armeabi-v7a/*test*.*
fi

if [ $BUILD_OK == "0" ] 
then
    # build apk && upload
    if [ $BUILD_TYPE == "release" ]; then
        ant "$VERBOSITY" release 
    else
        ant "$VERBOSITY" debug install
    fi
    BUILD_OK=$?
fi
    
cd -

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
    if [ $BUILD_TYPE == "release" ]; then
        echo "final release package can be found in bin/$PROJECT_NAME-release.apk"
    fi
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi

# removed copied assets from javas assets dir
rm -rf $PROJECT_NAME/assets/*

