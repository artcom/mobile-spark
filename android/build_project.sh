#! /bin/bash

source ./android/utils.sh


./android/c++build_project.sh $*
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
if [[ "`uname -s`" == *CYGWIN* ]]; then
    ANDROID_TOOL="android.bat"
fi

cd $SPARK_COMPONENT_DIR
# copy assets
if [ $DEPLOY == "1" ]
then
    JAVA_PROJECT_DIR=android/$SPARK_COMPONENT_NAME $MOBILE_SPARK/android/deploy_prepare.sh
fi
# package java
cd android
if [ $BUILD_OK == "0" ] 
then
    cd $SPARK_COMPONENT_NAME
    ## update Base project
    $ANDROID_TOOL update lib-project --target android-$ANDROID_LEVEL --path $MOBILE_SPARK/android/SparkViewerBase 
    # update android project
if [[ "`uname -s`" == *CYGWIN* ]]; then
    HELP=$(cygpath "$MOBILE_SPARK/android/SparkViewerBase")
else    
    HELP=$MOBILE_SPARK/android/SparkViewerBase
fi    
    # android bug: --library fails on absolute paths
    REL_DIR=$(get_relative_path `pwd` $HELP)
    $ANDROID_TOOL update project --target android-$ANDROID_LEVEL --name $SPARK_COMPONENT_NAME --path . 
    $ANDROID_TOOL update project --library $REL_DIR --target android-$ANDROID_LEVEL --name $SPARK_COMPONENT_NAME --path . 
    BUILD_OK=$?
fi

if [ $BUILD_OK == "0" ] 
then
    # build apk && upload
    if [ $BUILD_TYPE == "release" ]; then
        ant -Dnative.libs.absolute.dir="$MOBILE_SPARK/_build/lib" "$VERBOSITY" release 
    else
        ant -Dnative.libs.absolute.dir="$MOBILE_SPARK/_build/lib" "$VERBOSITY" debug install
    fi
    BUILD_OK=$?
fi
    
cd $MOBILE_SPARK

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
    if [ $BUILD_TYPE == "release" ]; then
        echo "final release package can be found in bin/$SPARK_COMPONENT_NAME-release.apk"
    fi
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi

if [ $DEPLOY == "1" ]
then
  JAVA_PROJECT_DIR=$SPARK_COMPONENT_DIR/android/$SPARK_COMPONENT_NAME $MOBILE_SPARK/android/deploy_cleanup.sh
fi

