// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "I18nShapeWidget.h"

namespace spark {
    class Image : public I18nShapeWidget {
    public:
        Image(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Image();
        const std::string & getSrc() const {return data_;};
        virtual void setSrc(const std::string & theSrc);
        const vector2 & getRealImageSize();
        virtual void setWidth(unsigned theWidth) {_myForcedSize[0] = theWidth;}
        virtual void setHeight(unsigned theHeight) {_myForcedSize[1] = theHeight;}

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Image::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        vector2 _myTextureSize;     // this is the size of the gl texture
        vector2 _myRealImageSize;   // this is the size of the asset image
        vector2 _myForcedSize;      // this the gl quad size
    };

    typedef masl::Ptr<Image> ImagePtr;
    typedef MemberFunctionEventCallback<Image, ImagePtr> ImageCB;
};
#endif
