#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_

#ifdef ANDROID
#include "JNIBinding.h"
#endif

#include <masl/Settings.h>
#include <masl/ThreadLock.h>

#include "Event.h"

namespace spark {

    class Window;
    typedef masl::Ptr<Window> WindowPtr;

    class BaseApp : public boost::enable_shared_from_this<BaseApp> {
        public:
            BaseApp(const std::string & theAppPath);
            virtual ~BaseApp();

            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);
            bool isSetup() const {return _mySetupFlag;}
            bool isSparkRealized() const {return _mySparkRealizedFlag;}
            void realize();
            void loadLayoutAndRegisterEvents(const std::string & theBaseName, int theScreenWidth, int theScreenHeight);
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
    std::string findBestMatchedLayout(std::string theBaseName, int theScreenWidth, int theScreenHeight, bool &isPortrait);
    void assetProviderSetup(const std::string & theAssetPath, const std::string & theAppPath );

    
    typedef masl::Ptr<BaseApp> BaseAppPtr;
};

#endif
