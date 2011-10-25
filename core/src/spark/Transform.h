// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Transform_
#define _included_mobile_spark_Transform_

#include "Widget.h"

namespace spark {

    class Transform : public Widget {
    public:
        Transform(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Transform();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Transform::SPARK_TYPE;};
    private:
    };

    typedef masl::Ptr<Transform> TransformPtr;
};

#endif
