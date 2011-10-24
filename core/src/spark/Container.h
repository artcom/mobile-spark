// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Container_
#define _included_mobile_spark_Container_

#include <string>
#include <vector>

#include "Component.h"

namespace spark {
    class BaseApp;
    typedef masl::Ptr<BaseApp> BaseAppPtr;

    class Container : public Component {
    public:
        Container(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Container() = 0;
        
        virtual void reparent();
        virtual VectorOfComponentPtr getChildrenByType(const std::string & theType) const; 
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const; 
        virtual void addChild(const ComponentPtr theChild);

        void removeChild(ComponentPtr theChild);
        const BaseAppPtr getApp() { return _myApp;}

    protected:
        const BaseAppPtr _myApp;


    };

    typedef masl::Ptr<Container> ContainerPtr;
};
#endif
