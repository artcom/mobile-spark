// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Image.h"

#include <mar/TextureLoader.h>
#include <mar/Shape.h>
#include <mar/Material.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

using namespace mar;

namespace spark {
    const char * const Image::SPARK_TYPE = "Image";

    Image::Image(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        I18nShapeWidget(theApp, theXMLNode)
    {
        _myForcedSize[0] = getNode()->getAttributeAs<float>("width", -1);
        _myForcedSize[1] = getNode()->getAttributeAs<float>("height", -1);
        _mipmap = getNode()->getAttributeAs<bool>("mipmap", false);
        setI18nData(getNode()->getAttributeAs<std::string>("src", ""));
    }

    Image::~Image() {
        AC_INFO << "....destructor image " << getName();
    }

    const vector2 &
    Image::getRealImageSize() {
        if (_myDirtyFlag) {
            build();
        }
        return _myRealImageSize;
    }
    
    void 
    Image::setSrc(const std::string & theSrc) { 
        AC_PRINT << "Image::setSrc : " << theSrc;
        
        _data = theSrc; 
        _myDirtyFlag = true;
    } 
    
    void
    Image::build() {
        I18nShapeWidget::build();
        if(_data.empty()) return;
        
        AC_DEBUG<<"build image " << *this << " with src: "<<_data;
        UnlitTexturedMaterialPtr myMaterial;
        bool myCacheFlag = getNode()->getAttributeAs<bool>("cache", false);
        
        if (!getShape()) {
            TexturePtr myTexture = TextureLoader::get().load(_data, myCacheFlag, _mipmap);
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(myTexture));
            myMaterial->setCustomHandles(customShaderValues_);
            myMaterial->setShader(vertexShader_, fragmentShader_); 
            _myShape = createCustomShape(myMaterial);
            
            if(_mipmap)
            {
//                uint potWidth = Texture::nextPowerOfTwo(myTexture->_width);
//                uint potHeight = Texture::nextPowerOfTwo(myTexture->_height);
                
                float factorW = myTexture->_real_width / (float) myTexture->_width;
                float factorH = myTexture->_real_height / (float) myTexture->_height;
                
                printf("real_width: %d, real_height: %d   --  potWidth: %d, potHeight: %d\n",
                       myTexture->_real_width, myTexture->_real_height,
                       myTexture->_width, myTexture->_height);
                
                printf("factorW: %.2f, factorH: %.2f\n",factorW, factorH);
                
                
                _myShape->setTexCoords(vector2(0, 0), vector2(factorW, 0),
                                       vector2(0, factorH), vector2(factorW, factorH));
            }
            
        } else {
            myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            //XXX:not caching always generates a new Texture, setSrc would be enough
            TexturePtr myTexture = TextureLoader::get().load(_data, myCacheFlag, _mipmap);
            myMaterial->getTextureUnit()->setTexture(myTexture);
        }
        _myTextureSize = vector2(myMaterial->getTextureUnit()->getTexture()->_width, myMaterial->getTextureUnit()->getTexture()->_height);
        _myRealImageSize = vector2(myMaterial->getTextureUnit()->getTexture()->_real_width, myMaterial->getTextureUnit()->getTexture()->_real_height);
        float myWidth = _myForcedSize[0] == -1 ? _myRealImageSize[0] : _myForcedSize[0];
        float myHeight = _myForcedSize[1] == -1 ? _myRealImageSize[1] : _myForcedSize[1];
        setSize(myWidth, myHeight);
    }
}
