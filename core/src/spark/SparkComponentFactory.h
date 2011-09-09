#ifndef _included_mobile_spark_SparkComponentFactory_
#define _included_mobile_spark_SparkComponentFactory_

#include <masl/Singleton.h>
#include <masl/Exception.h>
#include "BaseApp.h"

#include "Component.h"

namespace spark {

    DEFINE_EXCEPTION(SparkComponentException, Exception)
    DEFINE_EXCEPTION(UnknownComponentException, SparkComponentException)

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
        ComponentPtr createComponent(const std::string & theComponentName, const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent = ComponentPtr()) const;

        ComponentPtr loadSparkLayoutFromFile(const BaseAppPtr theApp, const std::string & thePath);
        ComponentPtr loadSparkLayoutFromString(const BaseAppPtr theApp, const std::string & theNode);

    private:
        CallbackMap _myCreateCallbackMap;
        void setupFactory();
    };
};
#endif 
