#include "NinePatch.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

namespace spark {
    const char * const NinePatch::SPARK_TYPE = "NinePatch";

    NinePatch::NinePatch(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        i18nHandler_ = I18nHandlerPtr(new I18nHandler(theXMLNode, "src"));
        edgeLeft_ = _myXMLNode->getAttributeAs<float>("edgeLeft", 0);
        edgeTop_ = _myXMLNode->getAttributeAs<float>("edgeTop", 0);
        edgeRight_ = _myXMLNode->getAttributeAs<float>("edgeRight", 0);
        edgeBottom_ = _myXMLNode->getAttributeAs<float>("edgeBottom", 0);

        if (i18nHandler_->data_.size() > 0) {
            _myDirtyFlag = true;
            build(); // virtual in ctor, dirty flag?!
        }
    }

    NinePatch::~NinePatch() {
    }

    void
    NinePatch::realize() {
        ShapeWidget::realize();
        i18nHandler_->realize(boost::static_pointer_cast<Widget>(shared_from_this()));
    }

    void
    NinePatch::onResume() {
        ShapeWidget::onResume();
        _myDirtyFlag = true;
    }

    void
    NinePatch::build() {
        ShapeWidget::build();
        //XXX: use empty()
        if(i18nHandler_->data_.size() == 0) return;
        float width = _myXMLNode->getAttributeAs<float>("width", -1);
        float height = _myXMLNode->getAttributeAs<float>("height", -1);

        setShape(ShapeFactory::get().createNinePatch(i18nHandler_->data_, edgeLeft_, edgeTop_, edgeRight_, edgeBottom_,
                    width >= 0 ? width : 0, height >= 0 ? height : 0));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
        if (width == -1 || height == -1) {
            width = width == -1 ? myMaterial->getTexture()->width_ : width;
            height = height == -1 ? myMaterial->getTexture()->height_ : height;
            getShape()->setDimensions(width, height);
        }
    }
}
