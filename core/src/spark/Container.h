#ifndef _included_mobile_spark_Container_
#define _included_mobile_spark_Container_

#include <string>
#include <vector>

#include "Component.h"
#include "I18nConstants.h"

namespace spark {
    class BaseApp;
    typedef boost::shared_ptr<BaseApp> BaseAppPtr;

    class I18nItem;
    typedef boost::shared_ptr<I18nItem> I18nItemPtr;
    class I18nContext;
    typedef boost::shared_ptr<I18nContext> I18nContextPtr;

    class Container : public Component {
    public:
        Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Container() = 0;
        
        virtual VectorOfComponentPtr getChildrenByType(const std::string & theType) const; 
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const; 
        virtual void addChild(const ComponentPtr theChild, const bool theSetParentFlag = true);

        void removeChild(ComponentPtr theChild);
        const BaseAppPtr getApp() { return _myApp;}

        const I18nContextPtr getI18nContext() const { return _myI18nContext; };

    protected:
        const BaseAppPtr _myApp;

        I18nContextPtr _myI18nContext;
        I18nItemPtr _myI18nItem;
        std::string _myI18nId;

        std::vector<I18nContextPtr> getI18nContexts(); 
        I18nItemPtr getI18nItemByName(const std::string & theName);
        LANGUAGE getLanguage();
        void switchLanguage(LANGUAGE theLanguage);

    private:
        void setI18nContextIfAvailable();

    };

    typedef boost::shared_ptr<Container> ContainerPtr;
};
#endif
