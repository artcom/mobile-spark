#include "DemoApp.h"

/////////////////// Application code, this should be in java or script language later...
namespace asl {

    DemoApp::DemoApp():BaseApp() {
    }
    DemoApp::~DemoApp() {
    }

}

/////////////////////////////////////////////////////////////////////////App-Instance
asl::DemoApp ourApp;

/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_BaseNativeLib_setup(JNIEnv * env, jobject obj,  
                                                                 jstring apkFile,
                                                                 jstring layoutFile);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_BaseNativeLib_onFrame(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_BaseNativeLib_onTouch(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_artcom_mobile_BaseNativeLib_setup(JNIEnv * env, jobject obj,  
                                                             jstring apkFile,
                                                             jstring layoutFile) {
    ourApp.setup(apkFile, layoutFile, env);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_BaseNativeLib_onFrame(JNIEnv * env, jobject obj) {
    ourApp.onFrame();
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_BaseNativeLib_onTouch(JNIEnv * env, jobject obj) {
    ourApp.onTouch();
}


