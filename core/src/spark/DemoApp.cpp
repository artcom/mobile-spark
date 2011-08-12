#include "DemoApp.h"

#include <mar/openGL_functions.h>
#include <spark/SparkComponentFactory.h>

#include <masl/Logger.h>

/////////////////// Application code, this should be in java or script language later...
namespace spark {
    DemoApp::DemoApp():BaseApp() {
    }
    DemoApp::~DemoApp() {
    }
    
#ifdef __ANDROID__
    bool DemoApp::setup(std::string apkPath, std::string layoutFile, JNIEnv* env) {
        AC_PRINT << "setup";

        printGLInfo();
        
        //load apk
        apkArchive = NULL;
        //jboolean isCopy;
        //const char* str = env->GetStringUTFChars(apkPath, &isCopy);
        android::loadAPK(&apkArchive, apkPath);

        //load layout
        //str = env->GetStringUTFChars(layoutFile, &isCopy); 
        window = boost::static_pointer_cast<spark::Window>(spark::SparkComponentFactory::loadSparkLayout(BaseAppPtr(this), layoutFile));
    
        return true;
    }
#endif    
}

/////////////////////////////////////////////////////////////////////////App-Instance
spark::DemoApp ourApp;

#ifdef __ANDROID__
    
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                                 jstring apkFile,
                                                                 jstring layoutFile);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jstring apkFile,
                                                             jstring layoutFile) {
    jboolean isCopy;
    const char* myApkFile = env->GetStringUTFChars(apkFile, &isCopy);                                                                    
    const char* myLayoutFile = env->GetStringUTFChars(layoutFile, &isCopy);                                                                    
                                                                
    ourApp.setup(myApkFile, myLayoutFile, env);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj) {
    ourApp.onFrame();
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj) {
    ourApp.onTouch();
}
#endif

