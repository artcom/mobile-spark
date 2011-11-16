// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Shape3D_
#define _included_mobile_spark_Shape3D_

#include "ShapeWidget.h"

namespace spark {
    class Shape3D : public ShapeWidget {
    public:
        Shape3D(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Shape3D();
        void onResume();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Shape3D::SPARK_TYPE;};
        virtual std::string getAttributesAsString() const;
        void setSrc(const std::string & theSrc);
    protected:
        virtual void build();
    private:
        std::string _mySrc;
    };

    typedef masl::Ptr<Shape3D> Shape3DPtr;
    typedef masl::WeakPtr<Shape3D> Shape3DWeakPtr;
};
#endif
