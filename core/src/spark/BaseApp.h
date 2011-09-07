#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_


#include <boost/smart_ptr/shared_ptr.hpp>
#include <mar/Canvas.h>

#include "Window.h"
#include "EventFactory.h"

#ifdef __ANDROID__
    #include <jni.h>
#endif


namespace spark {

    class BaseApp : public boost::enable_shared_from_this<BaseApp> {
        public: 
            BaseApp();
            virtual ~BaseApp();

            virtual bool setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile);
            virtual void onFrame(EventPtr theEvent);
            virtual void onSizeChanged(int theWidth, int theHeight);
            virtual void onPause();
            virtual void onResume();
            virtual void onEvent(std::string theEventString);

            spark::WindowPtr _mySparkWindow; 
            void renderText(std::string theMessage, int theOpenGLTextureId);

        private:
            JNIEnv * env;
            jobject obj;
            mar::CanvasPtr _myGLCanvas;      
    };

    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
};

#endif
