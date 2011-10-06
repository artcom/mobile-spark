#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "ShapeWidget.h"
#include "I18nShapeWidget.h"
#include "Event.h"

namespace spark {
    class Image : public I18nShapeWidget {
    public:
        Image(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Image();
        virtual void onResume();
        virtual void realize();
        void setSrc(const std::string & theSrc);
        void setSize(float theWidth, float theHeight) {_myWidth=theWidth; _myHeight=theHeight; realize();};
        void fitToSize(float theWidth, float theHeight);
        const vector2 & getTextureSize();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Image::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        vector2 _myTextureSize;   
        float _myWidth;
        float _myHeight;
        float _myTextureScaleX;
        float _myTextureScaleY;

    };

    typedef masl::Ptr<Image> ImagePtr;
    typedef MemberFunctionEventCallback<Image, ImagePtr> ImageCB;
};
#endif
