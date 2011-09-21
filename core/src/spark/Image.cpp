#include "Image.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

namespace spark {
    const char * const Image::SPARK_TYPE = "Image";

    Image::Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {
        _mySrc = _myXMLNode->getAttributeAs<std::string>("src","");
        if (_mySrc.size() > 0) {
            _myDirtyFlag = true;
            build();
        }
    }

    Image::~Image() {
    }

    void
    Image::realize() {
        if (_myI18nId.size() > 0) {
            attachToI18nItem();
        }
    }

    void
    Image::onResume() {
        ShapeWidget::onResume();
        _myDirtyFlag = true;
    }

    void
    Image::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        build();
    }

    void
    Image::build() {
        if (_myDirtyFlag) {
            _myDirtyFlag = false;
            if(_mySrc.size() == 0) return;
            float width = _myXMLNode->getAttributeAs<float>("width", -1);
            float height = _myXMLNode->getAttributeAs<float>("height", -1);
            setShape(ShapeFactory::get().createRectangle(true, width >= 0 ? width : 0, height >= 0 ? height : 0, _mySrc));
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
            if (width == -1 || height == -1) {
                width = width == -1 ? myMaterial->getTexture()->width_ : width;
                height = height == -1 ? myMaterial->getTexture()->height_ : height;
                //dimensions have to be set after image size is known
                getShape()->setDimensions(width, height);
            }
        }
    }

    void 
    Image::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        _mySrc = _myI18nItem->getLanguageData();
        AC_DEBUG << "on language switch src " << _mySrc;
        _myDirtyFlag = true;
    }

    void
    Image::attachToI18nItem() {
        ImagePtr myImage = boost::static_pointer_cast<Image>(shared_from_this());
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new ImageCB(myImage, &Image::handleI18nOnLanguageSwitch));
        if (_myI18nItem) {
            _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            _myI18nItem = I18nImagePtr();
        }
        if (_myI18nId.size() > 0) {
            _myI18nItem = getI18nItemByName(_myI18nId);
            if (!_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + _myI18nId, PLUS_FILE_LINE);
            }
            _myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            handleI18nOnLanguageSwitch();
        } else {
            _mySrc = "";
        }
        _myDirtyFlag = true;
    }
}
