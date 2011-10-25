// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Rectangle.h"

#include <mar/Shape.h>
#include <mar/Material.h>
#include "SparkComponentFactory.h"

namespace spark {

    const char * const Rectangle::SPARK_TYPE = "Rectangle";

    Rectangle::Rectangle(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode),
        _myColor(_myXMLNode->getAttributeAs<vector3>("color", vector3(1,1,1)))
    {
        mar::UnlitColoredMaterialPtr myMaterial = mar::UnlitColoredMaterialPtr(new mar::UnlitColoredMaterial());
        myMaterial->setDiffuseColor(_myColor);
        _myShape = mar::ShapePtr(new mar::RectangleShape(myMaterial));
        setSize(vector2(getNode()->getAttributeAs<float>("width"), getNode()->getAttributeAs<float>("height")));
    }

    Rectangle::~Rectangle() {
    }
    
    std::string 
    Rectangle::getAttributesAsString() const {
        return ShapeWidget::getAttributesAsString() + " color=\""+masl::as_string(_myColor)+"\"";
    }

}
