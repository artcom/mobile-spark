
#include "BaseApp.h"

#include <masl/XMLUtils.h>
#include <mar/openGL_functions.h>
#include <spark/SparkComponentFactory.h>


namespace spark {

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
        window = boost::static_pointer_cast<spark::Window>(spark::SparkComponentFactory::loadSparkLayout(BaseAppPtr(this), str));
    
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

        window->render();
    }

    void BaseApp::onTouch() {
        AC_PRINT << "onTOUCH";
        _myAnimate = !_myAnimate;
    }
}

