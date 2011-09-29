#ifndef _included_mobile_templateapp_TemplateApp_
#define _included_mobile_templateapp_TemplateApp_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>

#ifdef ANDROID
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif

namespace templateapp {
    class TemplateApp: public spark::BaseApp {
        public: 
            TemplateApp();
            virtual ~TemplateApp();
            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);

        private:
        	
    };

    typedef masl::Ptr<TemplateApp> TemplateAppPtr;
    typedef spark::MemberFunctionEventCallback<TemplateApp, TemplateAppPtr> TemplateAppEventCB;

};

#endif 

