#ifndef _included_mobile_spark_SparkComponentFactory_
#define _included_mobile_spark_SparkComponentFactory_

#include "../base/BaseApp.h"

#include "Component.h"

namespace spark {
    ComponentPtr createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent = ComponentPtr());
    ComponentPtr loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath);
};
#endif 
