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
        
        virtual void realize();
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
