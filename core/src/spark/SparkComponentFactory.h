#ifndef _included_mobile_spark_SparkComponentFactory_
#define _included_mobile_spark_SparkComponentFactory_

#include "BaseApp.h"

#include "Component.h"

namespace spark {
    class SparkComponentFactory {
    public:
        static ComponentPtr createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent = ComponentPtr());
        static ComponentPtr loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath);
    };
};
#endif 
