#include "AndroidAudioEngine.h"

#include <masl/AssetProvider.h>


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


namespace android {

	static jmethodID getMethodID(const std::string & theJavaActivity, const char *methodName, const char *paramCode) {
		jmethodID ret = 0;
		if (gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
			AC_ERROR << "Failed to get the environment using GetEnv()";
			return 0;
		}
		if (gJavaVM->AttachCurrentThread(&env, 0) < 0) {
			AC_ERROR << "Failed to get the environment using AttachCurrentThread()";
			return 0;
		}
		classOfSparkViewerActivity = env->FindClass(theJavaActivity.c_str());
		if (! classOfSparkViewerActivity) {
			AC_ERROR << "Failed to find class of " << theJavaActivity.c_str();
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


    AndroidAudioEngine::AndroidAudioEngine(const std::string & theJavaActivity)
        : AudioEngine(), javaActivity_(theJavaActivity)
    {
    }

    AndroidAudioEngine::~AndroidAudioEngine() {
    }

    unsigned int
    AndroidAudioEngine::playEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "play Effect " << myFoundFile;
		int ret = 0;
		// int playEffect(String)
		jmethodID playEffectMethodID = getMethodID(javaActivity_, "playEffect", "(Ljava/lang/String;)I");
		if (playEffectMethodID) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			ret = env->CallStaticIntMethod(classOfSparkViewerActivity, playEffectMethodID, StringArg);
		}
		return (unsigned int)ret;
    }
}

