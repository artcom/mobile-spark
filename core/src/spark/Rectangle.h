// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Rectangle_
#define _included_mobile_spark_Rectangle_

#include "ShapeWidget.h"

namespace spark {
    class Rectangle : public ShapeWidget {
    public:
        Rectangle(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Rectangle();
        
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Rectangle::SPARK_TYPE;};
        virtual std::string getAttributesAsString() const;
    private:
        vector3 _myColor;
    };

    typedef masl::Ptr<Rectangle> RectanglePtr;
};
#endif
