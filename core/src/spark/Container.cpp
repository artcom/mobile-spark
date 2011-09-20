#include "Container.h"

#include <masl/Logger.h>

#include "SparkComponentFactory.h"
#include "I18nContext.h"

using namespace masl;

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Component(theXMLNode, theParent), _myApp(theApp)
    {
        for (std::vector<XMLNodePtr>::iterator it = theXMLNode->children.begin(); it != theXMLNode->children.end(); ++it) {
            if ((*it)->nodeName == "Template") {
                continue;
            }
            ComponentPtr childComponent = SparkComponentFactory::get().createComponent(_myApp, *it, ComponentPtr(this));
            AC_DEBUG << "Container Constructor " << getName() << " add child " << childComponent->getName();
            addChild(childComponent, false);
        }
        setI18nContextIfAvailable();
    }

    Container::~Container() {
    }

    void
    Container::setI18nContextIfAvailable() {
        std::string myI18nContextName = _myXMLNode->getAttributeAs<std::string>("i18nContext", "");
        if (myI18nContextName.size() > 0) {
            ComponentPtr myComponent = getChildByName(myI18nContextName);
            I18nContextPtr myContext = boost::static_pointer_cast<I18nContext>(myComponent);
            if (myContext) {
                _myI18nContext = myContext;
            }
        }
    }

    VectorOfComponentPtr
    Container::getChildrenByType(const std::string & theType) const {
        VectorOfComponentPtr myResult;
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->getType() == theType) {
                myResult.push_back(*it);
            }
        }
        return myResult;
    }

    ComponentPtr
    Container::getChildByName(const std::string & theName, bool theDeepFlag) const {
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->getName() == theName) {
                return (*it);
            } else {
                if (theDeepFlag) {
                    ComponentPtr myChildTraversal = (*it)->getChildByName(theName, theDeepFlag);
                    if (myChildTraversal) {
                        return myChildTraversal;
                    }
                }
            }
        }
        return ComponentPtr();
    }

    void 
    Container::addChild(const ComponentPtr theChild, const bool theSetParentFlag) {
        _myChildren.push_back(theChild);
        //set parent is optional because this(!) can not be done from constructor
        if (theSetParentFlag) {
            theChild->setParent(shared_from_this());
        }
    }

    void
    Container::removeChild(ComponentPtr theComponent) {
        for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if (*it == theComponent) {
                _myChildren.erase(it);
                break;
            }
        }
    }

    std::vector<I18nContextPtr>  
    Container::getI18nContexts() {
        std::vector<I18nContextPtr> myContexts;
        ContainerPtr myCurrent =  boost::static_pointer_cast<Container>(shared_from_this());
        while (myCurrent) {
            if (myCurrent->getI18nContext()) {
                myContexts.push_back(myCurrent->getI18nContext());
            }
            myCurrent = boost::static_pointer_cast<Container>(myCurrent->getParent());
        }
        return myContexts;
    }

    I18nItemPtr
    Container::getI18nItemByName(const std::string & theName) {
        I18nItemPtr myI18nItem;
        std::vector<I18nContextPtr> myContexts = getI18nContexts();
        for (std::vector<I18nContextPtr>::iterator it = myContexts.begin(); it != myContexts.end(); ++it) {
            (*it)->setup();  //??? good idea? this avoids postrealize
            ComponentPtr myComponent = (*it)->getChildByName(theName);
            if (myComponent) {
                myI18nItem = boost::static_pointer_cast<I18nItem>(myComponent);
                if (myI18nItem) {
                    AC_PRINT << ".................found i18nItem";
                    return myI18nItem;
                }
            }
        }
        AC_PRINT << "------------------ no i18nItem found";
        return myI18nItem;
    }
}
