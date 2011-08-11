#ifndef _included_mobile_asl_BaseApp_
#define _included_mobile_asl_BaseApp_

#include <jni.h>
#include "../android/APK_functions.h"
#include "../spark/Window.h"

#include <boost/smart_ptr/shared_ptr.hpp>

namespace asl {
    class BaseApp {
        public: 
            BaseApp();
            virtual ~BaseApp();

            virtual bool setup(jstring apkPath, jstring layoutFile, JNIEnv* env);
            virtual void onFrame();
            virtual void onTouch();

            spark::WindowPtr window; 
            float step;
            zip* apkArchive;
        protected:
            bool _myAnimate;
    };

    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
};

#endif
