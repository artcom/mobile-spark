// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Text.h"

#include <masl/MobileSDK.h>
#include <masl/AssetProvider.h>
#include <masl/checksum.h>
#include <mar/Shape.h>
#include <mar/Material.h>
#include <mar/Texture.h>
#include <mar/TextureLoader.h>

#include "BaseApp.h"
#include "SparkComponentFactory.h"

#include <boost/progress.hpp>

using namespace mar;
using namespace masl;
using namespace std;

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
        _myTextStartPos(0),
        _myCacheFlag(_myXMLNode->getAttributeAs<bool>("cache", false))
    {
        setI18nData(getNode()->getAttributeAs<std::string>("text", ""));
        
        std::string myFontName = _myXMLNode->getAttributeAs<std::string>("font", "");
        setFont(myFontName);
        
        AC_INFO << "Text ctor: " << _myFontPath;
        mar::UnlitTexturedMaterialPtr myMaterial = mar::UnlitTexturedMaterialPtr(new mar::UnlitTexturedMaterial());
        myMaterial->getTextureUnit()->getTexture()->_transparency = true;
        myMaterial->setCustomHandles(_myCustomShaderValues);
        myMaterial->setShader(_myVertexShader, _myFragmentShader); 
        _myShape = mar::ShapePtr(new mar::RectangleShape(myMaterial));
    }

    Text::~Text() {
        AC_INFO << "....destructor text " << getName();
    }

    //TODO maybe remove textSize member
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
        _myTextStartPos = (theIndex <= getText().size() ? theIndex:0);
        AC_DEBUG << "incoming index : " << theIndex << "->" << _myTextStartPos;
    }    
    void
    Text::build() {
        I18nShapeWidget::build();
        AC_DEBUG << "build "<<*this << " caching: " << _myCacheFlag;
        AC_TRACE << "data: " << getText().substr(_myTextStartPos, getText().size());
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
        bool myCreateTextureFlag = true;            
        unsigned long myKey =  masl::initiateCRC32();
        if (_myCacheFlag) {
            appendCRC32(myKey, getText().substr(_myTextStartPos, getText().size()));
            appendCRC32(myKey, as_string(_myFontSize));
            appendCRC32(myKey, _myFontPath);
            appendCRC32(myKey, as_string(_myTextColor));
            TexturePtr myTexturePtr = TextureLoader::get().getTexture(myKey);
            if (myTexturePtr) {
                _myTextSize[0] = myTexturePtr->_width;
                _myTextSize[1] = myTexturePtr->_height;
                _myRenderedGlyphIndex = TextGlyphIndexMap::get().getIndex(myKey);
                setSize(_myTextSize[0], _myTextSize[1]);
                myMaterial->getTextureUnit()->setTexture(myTexturePtr);
                myCreateTextureFlag = false;  
            }      
        }
        if (myCreateTextureFlag) {
            TexturePtr myTexture;

            if (!_myCacheFlag) {
                myTexture = myMaterial->getTextureUnit()->getTexture();
            } else {
                myTexture = TexturePtr(new Texture());
            }
            masl::TextInfo myTextInfo = masl::MobileSDK_Singleton::get()
                                            .getNative()->renderText(getText(),
                                            myTexture->_textureId, _myFontSize,
                                            _myTextColor, _myMaxWidth, _myMaxHeight,
                                            _myTextAlign, _myFontPath, _myLineHeight,
                                            _myTextStartPos, myTexture->_mirrorFlag);                                             
            myMaterial->getTextureUnit()->setTexture(myTexture);
            _myTextSize[0] = myTextInfo.width;
            _myTextSize[1] = myTextInfo.height;
            _myRenderedGlyphIndex = myTextInfo.renderedGlyphIndex;
            setSize(_myTextSize[0], _myTextSize[1]);
            myTexture->_width =  myTextInfo.width;
            myTexture->_height = myTextInfo.height;
            myTexture->_textureId = myTextInfo.textureID;
            myTexture->_transparency = true;
            if (_myCacheFlag) {
                TextGlyphIndexMap::get().store(myKey, _myRenderedGlyphIndex);
                TextureLoader::get().storeTexture(myKey, myTexture);
            }            
        }
    }

    std::string 
    Text::getAttributesAsString() const {
        return I18nShapeWidget::getAttributesAsString() + " fontPath=\""+_myFontPath+"\"" + " textAlign=\""+_myTextAlign+"\""
            " fontSize=\""+masl::as_string(_myFontSize)+"\" textSize=\""+masl::as_string(_myTextSize)+"\""
            " cacheFlag=\""+masl::as_string(_myCacheFlag)+"\" textColor=\""+masl::as_string(_myTextColor)+"\""
            " maxWidth=\""+masl::as_string(_myMaxWidth)+"\" maxHeight=\""+masl::as_string(_myMaxHeight)+"\""
            " lineHeight=\""+masl::as_string(_myLineHeight)+"\"";
    }
    
    void Text::setColor(const vector4 &theColor){
        _myDirtyFlag = true;
        _myTextColor = theColor;
    }
    
    void Text::setFont(const string &theFont){
        
        _myDirtyFlag = true;
        
        string fontPath = masl::AssetProviderSingleton::get().ap()->findFile(theFont);
        if (fontPath != "") {
            _myFontPath = fontPath;
        }
    }

}
