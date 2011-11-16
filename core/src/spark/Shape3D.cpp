// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Shape3D.h"

#include <mar/ObjImporter.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    const char * const Shape3D::SPARK_TYPE = "Shape3D";

    Shape3D::Shape3D(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode),
        _mySrc(_myXMLNode->getAttributeAs<std::string>("src", ""))
    {}

    Shape3D::~Shape3D() {
    }

    void 
    Shape3D::setSrc(const std::string & theSrc) { 
        _mySrc = theSrc; _myDirtyFlag = true;
    } 

    void
    Shape3D::onResume() {
        ShapeWidget::onResume();
    }

    void
    Shape3D::build() {
        ShapeWidget::build();
        _myShape = mar::ShapePtr(new mar::ObjShape());
        mar::ObjImporter::get().importObj(_mySrc, _myShape);
    }

    std::string 
    Shape3D::getAttributesAsString() const {
        return ShapeWidget::getAttributesAsString() + " src=\""+_mySrc+"\"";
    }

}
