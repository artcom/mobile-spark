#ifndef _included_mobile_spark_Container_
#define _included_mobile_spark_Container_

#include <string>
#include <vector>

#include "Component.h"

namespace spark {
    class BaseApp;
    typedef boost::shared_ptr<BaseApp> BaseAppPtr;

    class Container : public Component {
    public:
        Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Container() = 0;
        
        virtual VectorOfComponentPtr getChildrenByType(const std::string & theType) const; 
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const; 
        virtual void addChild(ComponentPtr theChild);

        void removeChild(ComponentPtr theChild);
        const BaseAppPtr getApp() { return _myApp;}

    protected:
        const BaseAppPtr _myApp;
    private:

    };

    typedef boost::shared_ptr<Container> ContainerPtr;
};
#endif
