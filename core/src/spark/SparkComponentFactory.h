#ifndef _included_mobile_spark_SparkComponentFactory_
#define _included_mobile_spark_SparkComponentFactory_

#include <masl/Singleton.h>
#include <masl/Exception.h>
#include "BaseApp.h"

#include "Component.h"

namespace spark {

    DEFINE_EXCEPTION(SparkComponentException, Exception)
    DEFINE_EXCEPTION(UnknownComponentException, SparkComponentException)

    template < typename T>
    ComponentPtr create(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
        return ComponentPtr(new T(theApp, theXMLNode, theParent));
    };

    class SparkComponentFactory : public masl::Singleton<SparkComponentFactory> {
    public:
        SparkComponentFactory();
        ~SparkComponentFactory();
        typedef ComponentPtr (*CreateComponentCallback)(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
    private:
        typedef std::map<const std::string, CreateComponentCallback> CallbackMap;
    public:
        bool registerComponent(const std::string & theComponentName,
                        const CreateComponentCallback theCreateFn);
        bool unregisterComponent(const std::string & theComponentName);
        ComponentPtr createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent = ComponentPtr()) const;

        ComponentPtr loadSparkComponentsFromFile(const BaseAppPtr theApp, const std::string & thePath);
        ComponentPtr loadSparkComponentsFromString(const BaseAppPtr theApp, const std::string & theNode);

    private:
        CallbackMap createCallbackMap_;
        std::map<std::string, XMLNodePtr> templateMap_; //maps template name to rootXML of that template
        void setupFactory();
        void resolveTemplates(const BaseAppPtr theApp, XMLNodePtr theRoot);
        XMLNodePtr loadXMLNodeFromFile(const BaseAppPtr theApp, const std::string & thePath);
        XMLNodePtr loadXMLNodeFromString(const BaseAppPtr theApp, const std::string & theNode);
        XMLNodePtr prepareXMLFromString(const BaseAppPtr theApp, const std::string & theNode);
    };
};
#endif 
