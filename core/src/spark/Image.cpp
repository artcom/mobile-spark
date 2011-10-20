#include "Image.h"

#include <cml/mathlib/matrix_transform.h>
#include <mar/TextureLoader.h>
#include <mar/Shape.h>
#include <mar/Material.h>
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

    //XXX: onPause & onResume both unbind texture
    void
    Image::onPause() {
        I18nShapeWidget::onPause();
        if (getShape()) {
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            TextureUnitPtr myTextureUnit = myMaterial->getTextureUnit();
            myTextureUnit->getTexture()->unbind();
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

    //TODO maybe remove textureSize member
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

        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
        TextureUnitPtr myTextureUnit = myMaterial->getTextureUnit();
        cml::matrix_scale_2D(myTextureUnit->matrix_, scaleX/scale, scaleY/scale);
        setSize(theWidth, theHeight);
    }

    void
    Image::build() {
        I18nShapeWidget::build();
        if(data_.empty()) return;
        
        AC_DEBUG<<"build image " << *this << " with src: "<<data_;
        UnlitTexturedMaterialPtr myMaterial;
        bool myCacheFlag = getNode()->getAttributeAs<bool>("cache", false);
        if (!getShape()) {
            TexturePtr myTexture = TextureLoader::get().load(data_, myCacheFlag);
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(myTexture));
            myMaterial->setCustomHandles(customShaderValues_);
            myMaterial->setShader(vertexShader_, fragmentShader_); 
            _myShape = createCustomShape(myMaterial);
        } else {
            myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            //XXX:if not caching always generates a new Texture, setSrc would be enough
            TexturePtr myTexture = TextureLoader::get().load(data_, myCacheFlag);
            myMaterial->getTextureUnit()->setTexture(myTexture);
        }
        _myTextureSize = vector2(myMaterial->getTextureUnit()->getTexture()->width_, myMaterial->getTextureUnit()->getTexture()->height_);
        float myWidth = getNode()->getAttributeAs<float>("width", _myTextureSize[0]);
        float myHeight = getNode()->getAttributeAs<float>("height", _myTextureSize[1]);
        setSize(myWidth, myHeight);
    }
}
