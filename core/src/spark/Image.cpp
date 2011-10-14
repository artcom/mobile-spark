#include "Image.h"

#include <cml/mathlib/matrix_transform.h>
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
    }

    Image::~Image() {
        AC_INFO << "....destructor image " << getName();
    }

    void
    Image::realize() {
        I18nShapeWidget::realize();
    }

    void
    Image::onPause() {
        I18nShapeWidget::onPause();
        if (getShape()) {
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
            TexturePtr myTexture = myMaterial->getTexture();
            myTexture->unbind();
        }
    }

    void
    Image::onResume() {
        I18nShapeWidget::onResume();
        _myDirtyFlag = true;
        if (getShape()) {
            fitToSize(getShape()->getWidth(), getShape()->getHeight());
        }
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
        if (_myDirtyFlag) {
            build();
        }
        float scaleX = theWidth / _myTextureSize[0];
        float scaleY = theHeight / _myTextureSize[1];
        float scale = std::max(scaleX, scaleY);

        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
        TexturePtr myTexture = myMaterial->getTexture();
        cml::matrix_scale_2D(myTexture->matrix_, scaleX/scale, scaleY/scale);
        setSize(theWidth, theHeight);
    }

    
    void
    Image::build() {
        I18nShapeWidget::build();
        if(data_.empty()) return;
        
        AC_DEBUG<<"build image " << *this << " with src: "<<data_;
        std::vector<std::string> myHandles;
        myHandles.reserve(customShaderValues_.size());
        std::transform(customShaderValues_.begin(), customShaderValues_.end(), std::back_inserter(myHandles),
                       masl::select1st<std::map<std::string, float>::value_type>()) ;
        setShape(ShapeFactory::get().createRectangle(true, 1, 1, 
                                                     vertexShader_, fragmentShader_, myHandles, data_));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        _myTextureSize = vector2(myMaterial->getTexture()->width_, myMaterial->getTexture()->height_);
        float myWidth = getNode()->getAttributeAs<float>("width", _myTextureSize[0]);
        float myHeight = getNode()->getAttributeAs<float>("height", _myTextureSize[1]);
        setSize(myWidth, myHeight);
    }
}
