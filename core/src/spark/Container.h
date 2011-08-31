#ifndef _included_mobile_spark_Container_
#define _included_mobile_spark_Container_

#include <string>
#include <vector>

#include "Component.h"


namespace spark {
    class BaseApp;
    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
    typedef std::vector<ComponentPtr> VectorOfComponentPtr;
}

namespace spark {

    class Container : public Component {
    public: 
        Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Container() = 0;
        
        virtual void onPause() const ;
        virtual void onResume() const ;
        
        virtual VectorOfComponentPtr getChildrenByType(const std::string & theType) const; 
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const; 
        void insertChild(ComponentPtr theChild) { _myChildren.push_back(theChild);};
    protected:
        const BaseAppPtr _myApp;
        VectorOfComponentPtr _myChildren;
    private:

    };

    typedef boost::shared_ptr<Container> ContainerPtr;
};
#endif
