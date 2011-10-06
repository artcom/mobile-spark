#include "Image.h"

#include <masl/string_functions.h> //select1st
#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

using namespace mar;

namespace spark {
    const char * const Image::SPARK_TYPE = "Image";

    Image::Image(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        I18nShapeWidget(theApp, theXMLNode) 
    {
        setI18nData(getNode()->getAttributeAs<std::string>("src", ""));
        _myTextureScaleX=1;
        _myTextureScaleY=1;   
    }

    Image::~Image() {
    }

    void
    Image::realize() {
        I18nShapeWidget::realize();
    }

    void
    Image::onResume() {
        I18nShapeWidget::onResume();
        _myDirtyFlag = true;
    }

    const vector2 &
    Image::getTextureSize() {
        if (_myDirtyFlag) {
            build();
        }
        return _myTextureSize;
    }
    
    void 
    Image::setSrc(const std::string & theSrc) { 
        data_ = theSrc; _myDirtyFlag = true;
    } 
    
    void 
    Image::fitToSize(const float theWidth, const float theHeight) {
        vector2 textureSize = getTextureSize();
        float scaleX = theWidth / textureSize[0];
        float scaleY = theHeight / textureSize[1];
        float scale = std::max(scaleX, scaleY);
        _myTextureScaleX=scaleX/scale;
        _myTextureScaleY=scaleY/scale;
        setSize(theWidth, theHeight);
    }

    
    void
    Image::build() {
        I18nShapeWidget::build();
        if(data_.empty()) return;
        
        std::vector<std::string> myHandles;
        myHandles.reserve(customShaderValues_.size());
        std::transform(customShaderValues_.begin(), customShaderValues_.end(), std::back_inserter(myHandles),
                       masl::select1st<std::map<std::string, float>::value_type>()) ;
        setShape(ShapeFactory::get().createRectangle(true, _myWidth, _myHeight, 
                                                     vertexShader_, fragmentShader_, myHandles, data_,_myTextureScaleX,_myTextureScaleY));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        _myTextureSize = vector2(myMaterial->getTexture()->width_, myMaterial->getTexture()->height_);
        if (_myWidth == -1 || _myHeight == -1) {
            _myWidth = _myWidth == -1 ? myMaterial->getTexture()->width_ : _myWidth;
            _myHeight = _myHeight == -1 ? myMaterial->getTexture()->height_ : _myHeight;
            //dimensions have to be set after image size is known
            getShape()->setDimensions(_myWidth, _myHeight);
        }
    }
}
