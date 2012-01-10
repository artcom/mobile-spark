#!/bin/sh

MYDIR="$( cd "$( dirname "$0" )" && pwd )"

BUILD_DIR="${MYDIR}/build"

OUT_DIR="${MYDIR}/../_build/lib"
 
mkdir ${BUILD_DIR}
rm -rf ${BUILD_DIR}/*

mkdir "${OUT_DIR}/ios_Device"
mkdir "${OUT_DIR}/ios_Device/Release"
mkdir "${OUT_DIR}/ios_Device/Debug"

mkdir "${OUT_DIR}/ios_Simulator"
mkdir "${OUT_DIR}/ios_Simulator/Release"
mkdir "${OUT_DIR}/ios_Simulator/Debug"

# change into buildDir
cd ${BUILD_DIR}

# BUILD LIBRARIES for ios-DEVICE (armv7)
cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=${MYDIR}/../acmake/toolchains/ios.toolchain.iOS.Xcode.cmake ${MYDIR}/..

xcodebuild -project ${BUILD_DIR}/mobile-spark.xcodeproj/ -target ALL_BUILD -configuration Debug
mv ${MYDIR}/../_build/lib/*.a ${MYDIR}/../_build/lib/ios_Device/Debug

#xcodebuild -project ${BUILD_DIR}/mobile-spark.xcodeproj/ -target ALL_BUILD -configuration Release
#mv ${MYDIR}/../_build/lib/*.a ${MYDIR}/../_build/lib/ios_Device/Release

rm -rf ${BUILD_DIR}/* 

# build libraries for ios-SIMULATOR (i386)

cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=${MYDIR}/../acmake/toolchains/ios.toolchain.Simulator.Xcode.cmake ${MYDIR}/..

xcodebuild -project ${BUILD_DIR}/mobile-spark.xcodeproj/ -target ALL_BUILD -configuration Debug ARCHS=i386 ONLY_ACTIVE_ARCH=NO
mv ${MYDIR}/../_build/lib/*.a ${MYDIR}/../_build/lib/ios_Simulator/Debug

#xcodebuild -project ${BUILD_DIR}/mobile-spark.xcodeproj/ -target ALL_BUILD -configuration Release ARCHS=i386 ONLY_ACTIVE_ARCH=NO
#mv ${MYDIR}/../_build/lib/*.a ${MYDIR}/../_build/lib/ios_Simulator/Release

#rm -rf "${BUILD_DIR}/*"

# Done building ... now merge together all archs
for lib_i386_debug in `find ${OUT_DIR}/ios_Simulator/Debug -name "lib*\.a"`; do
	#echo ${lib_i386_debug}
		
	lib_i386_release=`echo $lib_i386_debug | sed "s/Debug/Release/g"`
	lib_arm7_debug=`echo $lib_i386_debug | sed "s/ios_Simulator/ios_Device/g"`
    lib_arm7_release=`echo $lib_arm7_debug | sed "s/Debug/Release/g"`
    lib=`echo $lib_i386_debug | sed "s/ios_Simulator\/Debug\///g"`
    
    lipoSucc=`lipo -arch armv7 $lib_arm7_debug -arch i386 $lib_i386_debug -create -output $lib`
    #lipoSucc=`lipo -arch armv7 $lib_arm7_release -arch i386 $lib_i386_release -create -output $lib`
done
	echo "success: $lipoSucc"
	
# CLEAN UP

rm -rf ${OUT_DIR}/ios_Simulator
rm -rf ${OUT_DIR}/ios_Device


