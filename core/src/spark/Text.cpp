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

namespace spark {
    const char * const Text::SPARK_TYPE = "Text";

    Text::Text(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        I18nShapeWidget(theApp, theXMLNode),
        _myFontSize(_myXMLNode->getAttributeAs<int>("fontsize", 32)),
        _myTextColor(_myXMLNode->getAttributeAs<vector4>("color", vector4(1,1,1,1))),
        _myMaxWidth(_myXMLNode->getAttributeAs<int>("maxWidth", 0)),
        _myMaxHeight(_myXMLNode->getAttributeAs<int>("maxHeight", 0)),
        _myTextAlign(_myXMLNode->getAttributeAs<std::string>("align", "left"))
    {
        setI18nData(getNode()->getAttributeAs<std::string>("text", ""));
        
        std::string myFontName = _myXMLNode->getAttributeAs<std::string>("font", "");
        if (myFontName != "") {
            _myFontPath = mar::AssetProviderSingleton::get().ap()->findFile(myFontName);
        }
        setShape(mar::ShapeFactory::get().createRectangle(true,500,500));
    }

    Text::~Text() {
    }

    void
    Text::realize() {
        I18nShapeWidget::realize();
    }
    
    
    void
    Text::onResume() {
        I18nShapeWidget::onResume();
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList[0]->material);
        myMaterial->getTexture()->setTextureId(0); //new texture should be generated
        _myDirtyFlag = true;
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
        I18nShapeWidget::build();
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList[0]->material);
        masl::TextInfo myTextInfo = masl::MobileSDK_Singleton::get().getNative()->renderText(data_, myMaterial->getTexture()->getTextureId(), _myFontSize,
                                         _myTextColor, _myMaxWidth, _myMaxHeight, _myTextAlign, _myFontPath);
        _myTextSize[0] = myTextInfo.width;
        _myTextSize[1] = myTextInfo.height;
        getShape()->setDimensions(_myTextSize[0], _myTextSize[1]);
        myMaterial->getTexture()->setTextureId(myTextInfo.textureID);
        myMaterial->transparency_ = true;
    }
}
