#ifndef _included_mobile_spark_SparkComponentFactory_
#define _included_mobile_spark_SparkComponentFactory_

#include <masl/Singleton.h>
#include "BaseApp.h"

#include "Component.h"

namespace spark {
    class SparkComponentFactory : public masl::Singleton<SparkComponentFactory> {
    public:
        typedef ComponentPtr (*CreateComponentCallback)(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
    private:
        typedef std::map<const std::string, CreateComponentCallback> CallbackMap;
    public:
        bool registerComponent(const std::string & theComponentName,
                        const CreateComponentCallback theCreateFn);
        bool unregisterComponent(const std::string & theComponentName);
        ComponentPtr createComponent(const std::string & theComponentName, const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent = ComponentPtr()) const;

        ComponentPtr loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath);

    private:
        CallbackMap _myCreateCallbackMap;
    };
};
#endif 
