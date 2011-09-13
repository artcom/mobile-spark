#ifndef _included_mobile_android_MobileSDK_
#define _included_mobile_android_MobileSDK_

#include <masl/MobileSDK.h>
#include <jni.h>    

namespace android {
    class AndroidMobileSDK : public masl::MobileSDK {
    public:
        AndroidMobileSDK();
        virtual ~AndroidMobileSDK();
        virtual masl::TextInfo renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight);
        virtual void updateCameraTexture();
        virtual masl::CameraInfo getCameraSpec();             
        virtual void startCameraCapture();
        virtual void stopCameraCapture();
        virtual bool isCameraCapturing();
        
        JNIEnv * env;
        jobject obj; 	
        
    };
    typedef boost::shared_ptr<AndroidMobileSDK> AndroidMobileSDKPtr;
};

#endif