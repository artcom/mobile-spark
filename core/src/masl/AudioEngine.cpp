#include "AudioEngine.h"

#include "AssetProvider.h"

static JavaVM *gJavaVM = 0;
static jclass classOfSparkViewerActivity = 0;
JNIEnv *env = 0;

#ifdef ANDROID
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    gJavaVM = vm;
    return JNI_VERSION_1_6;
}
#endif


namespace masl {

	static jmethodID getMethodID(const char *methodName, const char *paramCode) {
		jmethodID ret = 0;
		if (gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
			AC_ERROR << "Failed to get the environment using GetEnv()";
			return 0;
		}
		if (gJavaVM->AttachCurrentThread(&env, 0) < 0) {
			AC_ERROR << "Failed to get the environment using AttachCurrentThread()";
			return 0;
		}
		classOfSparkViewerActivity = env->FindClass("com/artcom/mobile/Base/SparkViewerActivity");
		if (! classOfSparkViewerActivity) {
			AC_ERROR << "Failed to find class of com/artcom/mobile/Base/SparkViewerActivity";
			return 0;
		}
		if (env != 0 && classOfSparkViewerActivity != 0) {
			ret = env->GetStaticMethodID(classOfSparkViewerActivity, methodName, paramCode);
		}
		if (! ret) {
			AC_ERROR << "get method id of " << methodName << " error";
		}
		return ret;
	}


    unsigned int
    AudioEngine::playEffect(const std::string & theFile) const {

        std::string myFoundFile = AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_PRINT << "play Effect " << myFoundFile;
		int ret = 0;
		// int playEffect(String)
		jmethodID playEffectMethodID = getMethodID("playEffect", "(Ljava/lang/String;)I");
        AC_PRINT << "method id " << playEffectMethodID;
		if (playEffectMethodID) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			ret = env->CallStaticIntMethod(classOfSparkViewerActivity, playEffectMethodID, StringArg);
		}
		return (unsigned int)ret;
    }
}

