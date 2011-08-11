#ifndef _included_mobile_spark_Container_
#define _included_mobile_spark_Container_

#include <string>
#include <vector>

#include "Component.h"



namespace asl {
    class BaseApp;
    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
}

namespace spark {

    class Container : public Component {
    public: 
        Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Container();
    protected:
        const BaseAppPtr _myApp;
        std::vector<ComponentPtr> _myChildren;
    private:

    };

    typedef boost::shared_ptr<Container> ContainerPtr;
};
#endif
