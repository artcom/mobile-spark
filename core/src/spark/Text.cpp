#include "Text.h"
#include "BaseApp.h"

#include <mar/AssetProvider.h>

#ifdef __ANDROID__
    #include <android/AndroidAssetProvider.h>
#endif
#if __APPLE__
    #include <ios/IOSAssetProvider.h>
#endif

#include "SparkComponentFactory.h"
#include <masl/MobileSDK.h>

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
}
