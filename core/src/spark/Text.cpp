#include "Text.h"

#include <masl/MobileSDK.h>
#include <mar/AssetProvider.h>

#ifdef __ANDROID__
    #include <android/AndroidAssetProvider.h>
#endif
#if __APPLE__
    #include <ios/IOSAssetProvider.h>
#endif

#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

using namespace std;
namespace spark {
    const char * const Text::SPARK_TYPE = "Text";

    Text::Text(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent), _myFontPath("") {

        _myText = _myXMLNode->getAttributeAs<std::string>("text", "");
        _myFontSize = _myXMLNode->getAttributeAs<int>("fontsize", 32);
        _myMaxWidth = _myXMLNode->getAttributeAs<int>("maxWidth", 0);
        _myMaxHeight = _myXMLNode->getAttributeAs<int>("maxHeight", 0);
        _myTextColor = _myXMLNode->getAttributeAs<vector4>("color", vector4(1,1,1,1));
        _myTextAlign = _myXMLNode->getAttributeAs<std::string>("align", "left");
        string myFontName = _myXMLNode->getAttributeAs<std::string>("font", "");
        if (myFontName != "") {
            _myFontPath = AssetProviderSingleton::get().ap()->findFile(myFontName);
         }
        setShape(ShapeFactory::get().createRectangle(true,500,500));
        _myDirtyFlag = true;
        build();
    }

    Text::~Text() {
    }

    void Text::realize() {
        if (_myI18nId.size() > 0) {
            attachToI18nItem();
        }
    }

    void
    Text::onResume() {
        ShapeWidget::onResume();
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
        myMaterial->getTexture()->setTextureId(0); //new texture should be generated
        _myDirtyFlag = true;
    }

    void
    Text::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        build();
    }
    const vector2 &
    Text::getTextSize() {
        if (_myDirtyFlag) {
            build();
        }
        return _myTextSize;
    }
    void
    Text::build() {
        if (_myDirtyFlag) {
            _myDirtyFlag = false;
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
            TextInfo myTextInfo = MobileSDK_Singleton::get().getNative()->renderText(_myText, myMaterial->getTexture()->getTextureId(), _myFontSize,
                                             _myTextColor, _myMaxWidth, _myMaxHeight, _myTextAlign, _myFontPath);

            _myTextSize[0] = myTextInfo.width;
            _myTextSize[1] = myTextInfo.height;
    		getShape()->setDimensions(_myTextSize[0], _myTextSize[1]);
    		//AC_PRINT << "rendered text :'" << _myText << "' has size: " << _myTextSize[0] << "/" << _myTextSize[1];
            myMaterial->getTexture()->setTextureId(myTextInfo.textureID);
            myMaterial->transparency_ = true;
        }
    }

    void 
    Text::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        _myText = boost::static_pointer_cast<I18nText>(_myI18nItem)->getText();
    }

    void
    Text::attachToI18nItem() {
        AC_PRINT << ".................attach to i18n " << getName();
        TextPtr myText = boost::static_pointer_cast<Text>(shared_from_this());
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new TextCB(myText, &Text::handleI18nOnLanguageSwitch));
        if (_myI18nItem) {
            _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            _myI18nItem = I18nTextPtr();
        }
        if (_myI18nId.size() > 0) {
            _myI18nItem = getI18nItemByName(_myI18nId);
            if (!_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + _myI18nId, PLUS_FILE_LINE);
            }
            _myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            handleI18nOnLanguageSwitch();
        } else {
            _myText = "";
        }
        _myDirtyFlag = true;
    }
}
