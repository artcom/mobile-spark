#include "NinePatch.h"

#include <mar/Shape.h>
#include <mar/Material.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

using namespace mar;

namespace spark {
    const char * const NinePatch::SPARK_TYPE = "NinePatch";

    NinePatch::NinePatch(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        I18nShapeWidget(theApp, theXMLNode),
        edgeLeft_(_myXMLNode->getAttributeAs<float>("edgeLeft", 0)),
        edgeTop_(_myXMLNode->getAttributeAs<float>("edgeTop", 0)),
        edgeRight_(_myXMLNode->getAttributeAs<float>("edgeRight", 0)),
        edgeBottom_(_myXMLNode->getAttributeAs<float>("edgeBottom", 0))
    {
        setI18nData(getNode()->getAttributeAs<std::string>("src", ""));        
    }

    NinePatch::~NinePatch() {
    }

    void
    NinePatch::realize() {
        I18nShapeWidget::realize();
    }

    void
    NinePatch::onResume() {
        I18nShapeWidget::onResume();
        _myDirtyFlag = true;
    }

    void
    NinePatch::build() {
        I18nShapeWidget::build();
        if(data_.empty()) return;

        UnlitTexturedMaterialPtr myMaterial;
        if (!getShape()) {
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(data_));
            myMaterial->setCustomHandles(customShaderValues_);
            myMaterial->setShader(vertexShader_, fragmentShader_); 
            NinePatchShapePtr myShape = NinePatchShapePtr(new NinePatchShape(myMaterial));
            myShape->setEdges(edgeLeft_, edgeTop_, edgeRight_, edgeBottom_);
            _myShape = myShape;
        } else {
            myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            myMaterial->getTexture()->getTextureInfo()->setSrc(data_);
        }

        float width = _myXMLNode->getAttributeAs<float>("width", myMaterial->getTexture()->getTextureInfo()->width_);
        float height = _myXMLNode->getAttributeAs<float>("height", myMaterial->getTexture()->getTextureInfo()->height_);
        setSize(width, height);
    }
}
