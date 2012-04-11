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
        I18nShapeWidget(theApp, theXMLNode),
        _mipmap(getNode()->getAttributeAs<bool>("mipmap", false))
    {
        setI18nData(getNode()->getAttributeAs<std::string>("src", ""));
    }

    Image::~Image() {
        AC_TRACE << "....delete image " << getName();
    }

    const vector2 &
    Image::getRealImageSize() {
        if (_myDirtyFlag) {
            build();
        }
        return _myRealImageSize;
    }
    
    void Image::setMipMap(bool theMipMapFlag) {
        AC_DEBUG << "Image::setMipMap : " << theMipMapFlag;
        if (_mipmap != theMipMapFlag) {
            _mipmap = theMipMapFlag;
            _myDirtyFlag = true;
        }
    }

    void Image::setSrc(const std::string & theSrc) { 
        AC_DEBUG << "Image::setSrc : " << theSrc;
        setI18nData(theSrc);
        _myDirtyFlag = true;
    } 
    
    void
    Image::build() {
        I18nShapeWidget::build();
        if(getSrc().empty()) return;
        
        AC_DEBUG<<"build image " << *this << " with src: "<<getSrc();
        UnlitTexturedMaterialPtr myMaterial;
        
        //XXX:not caching always generates a new Texture, setSrc would be enough
        bool myCacheFlag = getNode()->getAttributeAs<bool>("cache", false);
        TexturePtr myTexture = TextureLoader::get().load(getSrc(), myCacheFlag, _mipmap);
        
        if (!getShape()) {
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(myTexture));
            myMaterial->setCustomHandles(_myCustomShaderValues);
            myMaterial->setShader(_myVertexShader, _myFragmentShader); 
            _myShape = createCustomShape(myMaterial);
        } else {
            myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            myMaterial->getTextureUnit()->setTexture(myTexture);
        }
        _myTextureSize = vector2(myTexture->_width, myTexture->_height);
        _myRealImageSize = vector2(myTexture->_real_width, myTexture->_real_height);
        float myWidth = _myForcedSize[0] == -1 ? _myRealImageSize[0] : _myForcedSize[0];
        float myHeight = _myForcedSize[1] == -1 ? _myRealImageSize[1] : _myForcedSize[1];
        I18nShapeWidget::setSize(vector2(myWidth, myHeight));
    }
}
