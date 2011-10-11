#ifndef _included_mobile_ios_MobileSDK_
#define _included_mobile_ios_MobileSDK_

#include <masl/MobileSDK.h>
#include <masl/numeric_functions.h>


namespace ios {
    
    class IOSMobileSDK : public masl::MobileSDK {
    public:
        IOSMobileSDK();
        virtual ~IOSMobileSDK();
        virtual void vibrate(long theDurationMillisec);                            
        virtual masl::TextInfo renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, vector4 theColor, 
                                          int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath, 
                                          int theLineHeight, int theStartIndex);
        virtual void updateCameraTexture();
        virtual void freezeMobileOrientation(const std::string & theOrientation);
        virtual masl::CameraInfo getCameraSpec();
        virtual void startCameraCapture(bool theColorConversionFlag);
        virtual void stopCameraCapture();
        virtual bool isCameraCapturing();
        
    };

    typedef masl::Ptr<IOSMobileSDK> IOSMobileSDKPtr;
};

#endif
