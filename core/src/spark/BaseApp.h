#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_

#include <boost/smart_ptr/shared_ptr.hpp>

#ifdef __ANDROID__
#include "JNIBinding.h"
#endif

#include <masl/Settings.h>
#include <masl/ThreadLock.h>

#include "Event.h"

namespace spark {

//    class Dummy {
//        public:
//        Dummy();
//    };

    class Window;
    typedef boost::shared_ptr<Window> WindowPtr;

    class BaseApp : public boost::enable_shared_from_this<BaseApp> {
        public:
            BaseApp(const std::string & theAppPath);
            virtual ~BaseApp();

            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);
            bool isSetup() const {return _mySetupFlag;}
            bool isSparkRealized() const {return _mySparkRealizedFlag;}
            void realize();
            void loadLayoutAndRegisterEvents(const std::string & theBaseName, int theScreenWidth, int theScreenHeight);
            std::string findBestMatchedLayout(std::string theBaseName, int theScreenWidth, int theScreenHeight);
            virtual void onFrame(EventPtr theEvent);
            virtual void onPause(EventPtr theEvent);
            virtual void onResume();

            virtual void onEvent(std::string theEventString);
            virtual void handleEvents();
            void renderText(std::string theMessage, int theOpenGLTextureId);

            spark::WindowPtr _mySparkWindow;

        protected:
            std::string appPath_;
            bool _myChooseLayoutFlag;
            bool _mySetupFlag;       
            bool _mySparkRealizedFlag;  
        private:
            EventPtrList _myEvents;    
            masl::ThreadLock _myLock;
            
    };

    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
};

#endif
