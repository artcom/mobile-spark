#ifndef _ac_mobile_masl_MobileSDK_h_included_
#define _ac_mobile_masl_MobileSDK_h_included_

#include "Singleton.h"

namespace masl {
    struct CameraInfo{
        unsigned int textureID;
        int width;
        int height;
        int texturewidth;
        int textureheight;
    };
    struct TextInfo{
        unsigned int textureID;
        int width;
        int height;
        int renderedGlyphIndex;
    };

    class MobileSDK {
        public:
        virtual void vibrate(long theDurationMillisec) = 0;                                
        virtual TextInfo renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize,
                                    vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign,
                                    const std::string & theFontPath, int theLineHeight, int theStartIndex) = 0;
        virtual void updateCameraTexture() = 0;
        virtual void freezeMobileOrientation(const std::string & theOrientation) = 0;
        virtual CameraInfo getCameraSpec() = 0;
        virtual void startCameraCapture(bool theColorConversionFlag) = 0;
        virtual void stopCameraCapture() = 0;
        virtual bool isCameraCapturing() = 0;

    };

    typedef masl::Ptr<MobileSDK> MobileSDKPtr;

    class MobileSDK_Singleton : public Singleton<MobileSDK_Singleton> {
        public:
            virtual ~MobileSDK_Singleton();
            void setMobileSDK(MobileSDKPtr theMobileSDK);
            const MobileSDKPtr & getNative() const { return _myMobileSDK;};

        private:
            MobileSDKPtr _myMobileSDK;

    };

}
#endif
