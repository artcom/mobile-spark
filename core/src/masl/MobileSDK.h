/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
*/
#ifndef _ac_mobile_asl_MobileSDK_h_included_
#define _ac_mobile_asl_MobileSDK_h_included_

#include "Singleton.h"

namespace masl {
    struct CameraInfo{
        int textureID;
        int width;
        int height;
        int texturewidth;
        int textureheight;
    };
    struct TextInfo{
        int textureID;
        int width;
        int height;
    };
    
    class MobileSDK {
        public:
        virtual TextInfo renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath) = 0;
        virtual void updateCameraTexture() = 0;
        virtual void freezeMobileOrientation(std::string theOrientation) = 0;
        virtual CameraInfo getCameraSpec() = 0;              
        virtual void startCameraCapture(bool theColorConversionFlag) = 0;
        virtual void stopCameraCapture() = 0;
        virtual bool isCameraCapturing() = 0;
        
    };
    
    typedef boost::shared_ptr<MobileSDK> MobileSDKPtr;

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
