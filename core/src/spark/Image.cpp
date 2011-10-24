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
        setI18nData(getNode()->getAttributeAs<std::string>("src", ""));
    }

    Image::~Image() {
        AC_INFO << "....destructor image " << getName();
    }

    //TODO maybe remove textureSize member
    const vector2 &
    Image::getTextureSize() {
        if (_myDirtyFlag) {
            build();
        }
        return _myTextureSize;
    }
    
    void 
    Image::setSrc(const std::string & theSrc) { 
        data_ = theSrc; _myDirtyFlag = true;
    } 
    
    void
    Image::build() {
        I18nShapeWidget::build();
        if(data_.empty()) return;
        
        AC_DEBUG<<"build image " << *this << " with src: "<<data_;
        UnlitTexturedMaterialPtr myMaterial;
        bool myCacheFlag = getNode()->getAttributeAs<bool>("cache", false);
        if (!getShape()) {
            TexturePtr myTexture = TextureLoader::get().load(data_, myCacheFlag);
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(myTexture));
            myMaterial->setCustomHandles(customShaderValues_);
            myMaterial->setShader(vertexShader_, fragmentShader_); 
            _myShape = createCustomShape(myMaterial);
        } else {
            myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            //XXX:not caching always generates a new Texture, setSrc would be enough
            TexturePtr myTexture = TextureLoader::get().load(data_, myCacheFlag);
            myMaterial->getTextureUnit()->setTexture(myTexture);
        }
        _myTextureSize = vector2(myMaterial->getTextureUnit()->getTexture()->width_, myMaterial->getTextureUnit()->getTexture()->height_);
        float myWidth = getNode()->getAttributeAs<float>("width", _myTextureSize[0]);
        float myHeight = getNode()->getAttributeAs<float>("height", _myTextureSize[1]);
        setSize(myWidth, myHeight);
    }
}
