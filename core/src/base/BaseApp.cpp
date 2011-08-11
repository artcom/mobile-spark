
#include "BaseApp.h"

#include "XMLUtils.h"
#include "openGL_functions.h"
#include "../spark/SparkComponentFactory.h"


namespace asl {

    BaseApp::BaseApp(): step(0), _myAnimate(true) {
    }

    BaseApp::~BaseApp() {
    }

    bool BaseApp::setup(jstring apkPath, jstring layoutFile, JNIEnv* env) {
        AC_PRINT << "setup";

        printGLInfo();
        
        //load apk
        apkArchive = NULL;
        jboolean isCopy;
        const char* str = env->GetStringUTFChars(apkPath, &isCopy);
        android::loadAPK(&apkArchive, str);

        //load layout
        str = env->GetStringUTFChars(layoutFile, &isCopy); 
        AC_PRINT << "before factory call ";
        AC_PRINT << "path " << str;
        _myThis = BaseAppPtr(this);
        spark::ComponentPtr component = spark::loadSparkLayout(_myThis, str);
        AC_PRINT << "after factory call";
        //window = boost::static_pointer_cast<spark::Window>(component);

        return true;
    }

    void BaseApp::onFrame() {
        //AC_PRINT << "onFrame";
        if (_myAnimate) {
            step += 0.0001f;
            if (step > 1.0f) {
                step = 0.0f;
            }
        }
    }

    void BaseApp::onTouch() {
        AC_PRINT << "onTOUCH";
        _myAnimate = !_myAnimate;
    }
}

