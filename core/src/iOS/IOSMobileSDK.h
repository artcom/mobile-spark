#ifndef _included_mobile_ios_MobileSDK_
#define _included_mobile_ios_MobileSDK_

#include <masl/MobileSDK.h>
#include <masl/numeric_functions.h>

namespace ios {
    class IOSMobileSDK : public masl::MobileSDK {
    public:
        IOSMobileSDK();
        virtual ~IOSMobileSDK();
        virtual masl::TextInfo renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight);
        virtual void updateCameraTexture();
        virtual masl::CameraInfo getCameraSpec();             
        virtual void startCameraCapture();
        virtual void stopCameraCapture();
        virtual bool isCameraCapturing();
        
    private:
        //GLuint  texture;
            };
    typedef boost::shared_ptr<IOSMobileSDK> IOSMobileSDKPtr;
};

#endif