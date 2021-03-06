// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_MobileSDK_h_included_
#define _ac_mobile_masl_MobileSDK_h_included_

#include "Singleton.h"

namespace masl {
    struct VideoInfo{
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
#ifdef ANDROID        
        virtual int getApiVersion() = 0;                                
#endif                
        virtual TextInfo renderText(const std::string & theMessage, unsigned int theTextureId,
                                    int theFontSize, vector4 theColor, int theMaxWidth,
                                    int theMaxHeight, const std::string & theAlign,
                                    const std::string & theFontPath, int theLineHeight,
                                    int theStartIndex, bool & mirrorFlag) = 0;

        virtual bool loadTextureFromFile(const std::string & filename, unsigned int & textureId, 
                                         unsigned int & width, unsigned int & height, 
                                         unsigned int & real_width, unsigned int & real_height,
                                         matrix & npotMatrix,
                                         bool & hasAlpha, bool & theMipmapFlag) = 0;
        
        virtual void freezeMobileOrientation(const std::string & theOrientation) = 0;
        
        // Camera
        virtual void updateCameraTexture() = 0;
        virtual VideoInfo getCameraSpec() = 0;
        virtual void startCameraCapture(bool theColorConversionFlag) = 0;
        virtual void stopCameraCapture() = 0;
        virtual bool isCameraCapturing() = 0;
        
        virtual void exit() = 0;
        
        virtual std::string getOrientation() = 0;
        
        virtual float getDeviceBatteryLevel() = 0;
        

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
