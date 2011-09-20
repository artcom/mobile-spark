#ifndef _included_mobile_ios_MobileSDK_
#define _included_mobile_ios_MobileSDK_

#include <masl/MobileSDK.h>
#include <masl/numeric_functions.h>

namespace ios {
    class IOSMobileSDK : public masl::MobileSDK {
    public:
        IOSMobileSDK();
        virtual ~IOSMobileSDK();
        virtual masl::TextInfo renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath);
        virtual void updateCameraTexture();
        virtual void freezeMobileOrientation(std::string theOrientation);
        virtual masl::CameraInfo getCameraSpec();
        virtual void startCameraCapture(bool theColorConversionFlag);
        virtual void stopCameraCapture();
        virtual bool isCameraCapturing();

    private:
            };
    typedef boost::shared_ptr<IOSMobileSDK> IOSMobileSDKPtr;
};

#endif
