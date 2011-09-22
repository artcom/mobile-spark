#include "NinePatch.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

namespace spark {
    const char * const NinePatch::SPARK_TYPE = "NinePatch";

    NinePatch::NinePatch(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        src_ = _myXMLNode->getAttributeAs<std::string>("src","");
        edgeLeft_ = _myXMLNode->getAttributeAs<float>("edgeLeft", 0);
        edgeTop_ = _myXMLNode->getAttributeAs<float>("edgeTop", 0);
        edgeRight_ = _myXMLNode->getAttributeAs<float>("edgeRight", 0);
        edgeBottom_ = _myXMLNode->getAttributeAs<float>("edgeBottom", 0);
        if (src_.size() < 0) {
            _myDirtyFlag = true;
            build();
        }
    }

    NinePatch::~NinePatch() {
    }

    void
    NinePatch::realize() {
        if (_myI18nId.size() > 0) {
            attachToI18nItem();
        }
    }

    void
    NinePatch::onResume() {
        ShapeWidget::onResume();
        _myDirtyFlag = true;
    }

    void
    NinePatch::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        build();
    }

    void
    NinePatch::build() {
        if (_myDirtyFlag) {
            _myDirtyFlag = false;
            float width = _myXMLNode->getAttributeAs<float>("width", -1);
            float height = _myXMLNode->getAttributeAs<float>("height", -1);

            setShape(ShapeFactory::get().createNinePatch(src_, edgeLeft_, edgeTop_, edgeRight_, edgeBottom_,
                        width >= 0 ? width : 0, height >= 0 ? height : 0));
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
            if (width == -1 || height == -1) {
                width = width == -1 ? myMaterial->getTexture()->width_ : width;
                height = height == -1 ? myMaterial->getTexture()->height_ : height;
                getShape()->setDimensions(width, height);
            }
        }
    }

    void 
    NinePatch::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        src_ = _myI18nItem->getLanguageData();
        AC_DEBUG << "on language switch src " << src_;
        _myDirtyFlag = true;
    }

    void
    NinePatch::attachToI18nItem() {
        NinePatchPtr myNinePatch = boost::static_pointer_cast<NinePatch>(shared_from_this());
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new NinePatchCB(myNinePatch, &NinePatch::handleI18nOnLanguageSwitch));
        if (_myI18nItem) {
            _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            _myI18nItem = I18nItemPtr();
        }
        if (_myI18nId.size() > 0) {
            _myI18nItem = getI18nItemByName(_myI18nId);
            if (!_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + _myI18nId, PLUS_FILE_LINE);
            }
            _myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            handleI18nOnLanguageSwitch();
        } else {
            src_ = "";
        }
        _myDirtyFlag = true;
    }
}
