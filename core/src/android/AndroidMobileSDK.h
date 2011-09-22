#ifndef _included_mobile_android_MobileSDK_
#define _included_mobile_android_MobileSDK_

#include <masl/MobileSDK.h>
#include <jni.h>

namespace android {
    class AndroidMobileSDK : public masl::MobileSDK {
    public:
        AndroidMobileSDK();
        virtual ~AndroidMobileSDK();
        virtual void vibrate(long theDurationMillisec);                
        virtual masl::TextInfo renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath);
        virtual void updateCameraTexture();
        virtual void freezeMobileOrientation(std::string theOrientation);
        virtual masl::CameraInfo getCameraSpec();
        virtual void startCameraCapture(bool theColorConversionFlag);
        virtual void stopCameraCapture();
        virtual bool isCameraCapturing();

        JNIEnv * env;
        jobject obj;

    };
    typedef boost::shared_ptr<AndroidMobileSDK> AndroidMobileSDKPtr;
};

#endif
