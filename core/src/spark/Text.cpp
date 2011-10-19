#include "Text.h"

#include <masl/MobileSDK.h>
#include <masl/AssetProvider.h>
#include <masl/checksum.h>

#include <mar/Texture.h>
#include <mar/TextureLoader.h>

#include "BaseApp.h"
#include "SparkComponentFactory.h"
#include "I18nContext.h"

#include <boost/progress.hpp>

using namespace mar;
using namespace masl;

namespace spark {
    
    TextGlyphIndexMap::TextGlyphIndexMap() {}
    TextGlyphIndexMap::~TextGlyphIndexMap() {}
    void TextGlyphIndexMap::store(const unsigned long theKey, int myRenderedGlyphIndex) {_myRenderedGlyphTextureMap[theKey] =  myRenderedGlyphIndex;}         
    int TextGlyphIndexMap::getIndex(const unsigned long theKey)  {
        return  (_myRenderedGlyphTextureMap.find(theKey) != _myRenderedGlyphTextureMap.end()) ? _myRenderedGlyphTextureMap[theKey] : -1;
    }       
        
        
    const char * const Text::SPARK_TYPE = "Text";
    unsigned Text::ourTextCounter = 0;
    Text::Text(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        I18nShapeWidget(theApp, theXMLNode),
        _myFontSize(_myXMLNode->getAttributeAs<int>("fontsize", 32)),
        _myTextColor(_myXMLNode->getAttributeAs<vector4>("color", vector4(1,1,1,1))),
        _myMaxWidth(_myXMLNode->getAttributeAs<int>("maxWidth", 0)),
        _myMaxHeight(_myXMLNode->getAttributeAs<int>("maxHeight", 0)),
        _myLineHeight(_myXMLNode->getAttributeAs<int>("lineHeight", 0)),
        _myTextAlign(_myXMLNode->getAttributeAs<std::string>("align", "left")),
        _myRenderedGlyphIndex(0),
        _myTextStartPos(0), _myCacheFlag(_myXMLNode->getAttributeAs<bool>("cache", false))
    {
        setI18nData(getNode()->getAttributeAs<std::string>("text", ""));
        
        std::string myFontName = _myXMLNode->getAttributeAs<std::string>("font", "");
        if (myFontName != "") {
            _myFontPath = masl::AssetProviderSingleton::get().ap()->findFile(myFontName);
        }
        setShape(mar::ShapeFactory::get().createRectangle(true,500,500));
    }

    Text::~Text() {
        AC_INFO << "....destructor text " << getName();
    }

    void
    Text::realize() {
        I18nShapeWidget::realize();
    }
    
    void
    Text::onPause() {
        I18nShapeWidget::onPause();
        if (getShape()) {
            mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material);
            mar::TexturePtr myTexture = myMaterial->getTexture();
            myTexture->getTextureInfo()->unbind();
        }
    }
    
    void
    Text::onResume() {
        I18nShapeWidget::onResume();
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material);
        _myDirtyFlag = true;
    }

    const vector2 &
    Text::getTextSize() {
        if (_myDirtyFlag) {
            build();
        }
        return _myTextSize;
    }
    int 
    Text::getRenderedGlyphIndex() { 
        if (_myDirtyFlag) {
            build();
        }
        return _myRenderedGlyphIndex;
    }
        
    void
    Text::setStartIndex(unsigned int theIndex) {
        _myTextStartPos = (theIndex<=data_.size() ? theIndex:0);
        AC_DEBUG << "incoming index : " << theIndex << "->" << _myTextStartPos;
    }    
    void
    Text::build() {
        I18nShapeWidget::build();
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material);
        bool myCreateTextureFlag = true;            
        unsigned long myKey =  masl::initiateCRC32();
        if (_myCacheFlag) {
            appendCRC32(myKey, data_.substr(_myTextStartPos, data_.size()));
            appendCRC32(myKey, as_string(_myFontSize));
            appendCRC32(myKey, _myFontPath);
            appendCRC32(myKey, as_string(_myTextColor));
            TextureInfoPtr myTexturePtr = TextureLoader::get().getTextureInfo(myKey);
            if (myTexturePtr) {
                _myTextSize[0] = myTexturePtr->width_;
                _myTextSize[1] = myTexturePtr->height_;
                _myRenderedGlyphIndex = TextGlyphIndexMap::get().getIndex(myKey);
                setSize(_myTextSize[0], _myTextSize[1]);
                myMaterial->getTexture()->getTextureInfo()->textureId_ = myTexturePtr->textureId_;        
                myCreateTextureFlag = false;  
            }      
        }
        if (myCreateTextureFlag) {
            masl::TextInfo myTextInfo = masl::MobileSDK_Singleton::get().getNative()->renderText(data_, 0, _myFontSize,
                                             _myTextColor, _myMaxWidth, _myMaxHeight, _myTextAlign, _myFontPath, _myLineHeight, _myTextStartPos);                                             
            _myTextSize[0] = myTextInfo.width;
            _myTextSize[1] = myTextInfo.height;
            _myRenderedGlyphIndex = myTextInfo.renderedGlyphIndex;
            setSize(_myTextSize[0], _myTextSize[1]);
            myMaterial->getTexture()->getTextureInfo()->width_ =  myTextInfo.width;                
            myMaterial->getTexture()->getTextureInfo()->height_ = myTextInfo.height;                
            myMaterial->getTexture()->getTextureInfo()->textureId_ = myTextInfo.textureID;    
            if (_myCacheFlag) {
                TextGlyphIndexMap::get().store(myKey, _myRenderedGlyphIndex);
                TextureLoader::get().storeTextureInfo(myKey, myMaterial->getTexture()->getTextureInfo());
            }            
        }
        myMaterial->transparency_ = true;
    }

    std::string 
    Text::getAttributesAsString() const {
        return I18nShapeWidget::getAttributesAsString() + " fontPath=\""+_myFontPath+"\"" + " textAlign=\""+_myTextAlign+"\""
            " fontSize=\""+masl::as_string(_myFontSize)+"\" textSize=\""+masl::as_string(_myTextSize)+"\""
            " cacheFlag=\""+masl::as_string(_myCacheFlag)+"\" textColor=\""+masl::as_string(_myTextColor)+"\""
            " maxWidth=\""+masl::as_string(_myMaxWidth)+"\" maxHeight=\""+masl::as_string(_myMaxHeight)+"\""
            " lineHeight=\""+masl::as_string(_myLineHeight)+"\"";
    }

}
