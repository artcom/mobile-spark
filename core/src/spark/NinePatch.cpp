#include "NinePatch.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

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

        setShape(mar::ShapeFactory::get().createNinePatch(data_, edgeLeft_, edgeTop_, edgeRight_, edgeBottom_, 100, 100));
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material);

        float width = _myXMLNode->getAttributeAs<float>("width", myMaterial->getTexture()->width_);
        float height = _myXMLNode->getAttributeAs<float>("height", myMaterial->getTexture()->height_);
        setSize(width, height);
    }
}
