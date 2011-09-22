#include "Image.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

namespace spark {
    const char * const Image::SPARK_TYPE = "Image";

    Image::Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode) {
        i18nHandler_ = I18nHandlerPtr(new I18nHandler(theXMLNode, "src"));
    }

    Image::~Image() {
    }

    void
    Image::realize() {
        ShapeWidget::realize();
        i18nHandler_->realize(boost::static_pointer_cast<Widget>(shared_from_this()));
    }

    void
    Image::onResume() {
        ShapeWidget::onResume();
        _myDirtyFlag = true;
    }

    
    void
    Image::build() {
        ShapeWidget::build();
        if(i18nHandler_->data_.empty()) return;
        float width = _myXMLNode->getAttributeAs<float>("width", -1);
        float height = _myXMLNode->getAttributeAs<float>("height", -1);
        setShape(ShapeFactory::get().createRectangle(true, width >= 0 ? width : 0, height >= 0 ? height : 0, i18nHandler_->data_));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        if (width == -1 || height == -1) {
            width = width == -1 ? myMaterial->getTexture()->width_ : width;
            height = height == -1 ? myMaterial->getTexture()->height_ : height;
            //dimensions have to be set after image size is known
            getShape()->setDimensions(width, height);
        }
    }
}
