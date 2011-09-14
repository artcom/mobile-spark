#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_


#include <boost/smart_ptr/shared_ptr.hpp>
//#include <mar/Canvas.h>
#ifdef __ANDROID__
#include "JNIBinding.h"
#endif

#include "Window.h"
#include "EventFactory.h"


namespace spark {

    class BaseApp : public boost::enable_shared_from_this<BaseApp> {
        public: 
            BaseApp(const std::string & theAppPath);
            virtual ~BaseApp();

            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath);
            void loadLayoutAndRegisterEvents(const std::string & theLayoutFile);
            virtual void onFrame(EventPtr theEvent);
            virtual void onPause(EventPtr theEvent);
            virtual void onResume();
            virtual void onEvent(std::string theEventString);

            spark::WindowPtr _mySparkWindow; 
            void renderText(std::string theMessage, int theOpenGLTextureId);

        protected:
            std::string appPath_;
    };

    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
};

#endif
