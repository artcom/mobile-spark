#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_

//#define BOOST_NO_SFINAE
//#include <boost/thread/pthread/shared_mutex.hpp>
//#include <boost/thread/locks.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

//#include <mar/Canvas.h>
#ifdef __ANDROID__
#include "JNIBinding.h"
#endif

#include <masl/ThreadLock.h>

#include "Window.h"
#include "EventFactory.h"


namespace spark {

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
            spark::WindowPtr _mySparkWindow;
            void renderText(std::string theMessage, int theOpenGLTextureId);

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
