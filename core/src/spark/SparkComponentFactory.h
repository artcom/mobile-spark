// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_SparkComponentFactory_
#define _included_mobile_spark_SparkComponentFactory_

#include <masl/Singleton.h>
#include <masl/Exception.h>
#include "BaseApp.h"

#include "Component.h"
#include "Container.h"

namespace spark {

    DECLARE_EXCEPTION(SparkComponentException, masl::Exception)
    DECLARE_EXCEPTION(UnknownComponentException, SparkComponentException)

    template < typename T>
    ComponentPtr create(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode) {
        return ComponentPtr(new T(theApp, theXMLNode));
    };

    class SparkComponentFactory : public masl::Singleton<SparkComponentFactory> {
    public:
        SparkComponentFactory();
        ~SparkComponentFactory();
        typedef ComponentPtr (*CreateComponentCallback)(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
    private:
        typedef std::map<const std::string, CreateComponentCallback> CallbackMap;
    public:
        bool registerComponent(const std::string & theComponentName,
                        const CreateComponentCallback theCreateFn);
        bool unregisterComponent(const std::string & theComponentName);
        ComponentPtr createComponent(const BaseAppPtr theApp, const masl::XMLNodePtr theNode) const;

        ComponentPtr loadSparkComponentsFromFile(const BaseAppPtr theApp, const std::string & thePath, 
                                                 const ContainerPtr theParent = ContainerPtr());
        ComponentPtr loadSparkComponentsFromString(const BaseAppPtr theApp, const std::string & theNode,
                                                   const ContainerPtr theParent = ContainerPtr());
        std::vector<std::string> createSrcListFromSpark(const std::string & theSparkString);

    private:
        CallbackMap createCallbackMap_;
        std::map<std::string, masl::XMLNodePtr> templateMap_; //maps template name to rootXML of that template
        void setupFactory();
        void resolveTemplates(const BaseAppPtr theApp, masl::XMLNodePtr theRoot);
        masl::XMLNodePtr loadXMLNodeFromFile(const BaseAppPtr theApp, const std::string & thePath);
        masl::XMLNodePtr loadXMLNodeFromString(const BaseAppPtr theApp, const std::string & theNode);
    };
};
#endif
