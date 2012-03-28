// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_android_MobileSDK_
#define _included_mobile_android_MobileSDK_

#include <masl/MobileSDK.h>
#include <masl/Ptr.h>
#include <jni.h>

namespace android {
    class AndroidMobileSDK : public masl::MobileSDK {
    public:
        AndroidMobileSDK();
        virtual ~AndroidMobileSDK();
        virtual void vibrate(long theDurationMillisec);                
        virtual masl::TextInfo renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, vector4 theColor, 
                                          int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath, 
                                          int theLineHeight, int theStartIndex, bool & mirrorFlag);
        virtual bool loadTextureFromFile(const std::string & filename, unsigned int & textureId,
                                         unsigned int & width, unsigned int & height,
                                         unsigned int & real_width, unsigned int & real_height,
                                         matrix & npotMatrix,
                                         bool & hasAlpha, bool & theMipmapFlag);
        virtual void updateCameraTexture();
        virtual void freezeMobileOrientation(const std::string & theOrientation);
        virtual masl::VideoInfo getCameraSpec();
        virtual void startCameraCapture(bool theColorConversionFlag);
        virtual void stopCameraCapture();
        virtual bool isCameraCapturing();
        
        virtual void exit();

    	virtual std::string getOrientation();

        float getDeviceBatteryLevel();
    	
        JNIEnv * env;
        jobject obj;

    };
    typedef masl::Ptr<AndroidMobileSDK> AndroidMobileSDKPtr;
};

#endif
