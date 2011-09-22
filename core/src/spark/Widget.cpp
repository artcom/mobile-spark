#include "Widget.h"

#include "I18nContext.h"
#include "BaseApp.h"

namespace spark {
    Widget::Widget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Container(theApp, theXMLNode, theParent), _alpha(1.0), _actualAlpha(1.0), _visible(true), _sensible(true)
    {
        _x = _myXMLNode->getAttributeAs<float>("x", 0);
        _y = _myXMLNode->getAttributeAs<float>("y", 0);
        _z = _myXMLNode->getAttributeAs<float>("z", 0);
        _scaleX = _myXMLNode->getAttributeAs<float>("scaleX",1);
        _scaleY = _myXMLNode->getAttributeAs<float>("scaleY",1);
        _scaleZ = _myXMLNode->getAttributeAs<float>("scaleZ",1);
        _rotationX = _myXMLNode->getAttributeAs<float>("rotationX", 0);
        _rotationY = _myXMLNode->getAttributeAs<float>("rotationY", 0);
        _rotationZ = _myXMLNode->getAttributeAs<float>("rotationZ", 0);
        _visible = _myXMLNode->getAttributeAs<bool>("visible", _visible);
        _sensible = _myXMLNode->getAttributeAs<bool>("sensible", _sensible);

        updateMatrix();
        setI18nContextIfAvailable();
    }

    Widget::~Widget() {
    }

    void
    Widget::realize() {
        setAlpha(_myXMLNode->getAttributeAs<float>("alpha", _alpha));
    }

    void Widget::updateMatrix() {
        MatrixStack helpMatrixStack;
        helpMatrixStack.loadIdentity();
        helpMatrixStack.rotateXAxis(_rotationX);
        helpMatrixStack.rotateYAxis(_rotationY);
        helpMatrixStack.rotateZAxis(_rotationZ);
        helpMatrixStack.translate(_x, _y, _z);
        helpMatrixStack.scale(_scaleX, _scaleY, _scaleZ);
        _myLocalMatrix = helpMatrixStack.getTop();
    };

    //TODO: use visitor?
    void Widget::prerender(MatrixStack& theCurrentMatrixStack) {
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrix(_myLocalMatrix);
        _myWorldMVMatrix = theCurrentMatrixStack.getTop();
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->prerender(theCurrentMatrixStack);
        }
        theCurrentMatrixStack.pop();
        if (_myDirtyFlag) {
            build();
            setAlpha(_alpha);
            _myDirtyFlag = false;
        }
    }

    bool Widget::isRendered() const {
        if (!_visible) {
            return false;
        } else {
            if (getParent()) {
                return getParent()->isRendered();
            } else {
                return _visible;
            }
        }
    }

    void Widget::render(const matrix & theProjectionMatrix) const {
    }

    float
    Widget::getParentAlpha() const {
        float myParentAlpha = 1.0;
        if (getParent()) {
            WidgetPtr myParent = boost::dynamic_pointer_cast<Widget>(getParent());
            if (myParent) {
                myParentAlpha = myParent->getActualAlpha();
            }
        }
        return myParentAlpha;
    }

    void
    Widget::propagateAlpha() {
        _actualAlpha = getParentAlpha() * _alpha;
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            WidgetPtr myChild = boost::dynamic_pointer_cast<Widget>(*it);
            if (myChild) {
                myChild->propagateAlpha();
            }
        }
    }

    void
    Widget::setI18nContextIfAvailable() {
        std::string myI18nContextName = _myXMLNode->getAttributeAs<std::string>("i18nContext", "");
        if (myI18nContextName.size() > 0) {
            ComponentPtr myComponent = getChildByName(myI18nContextName);
            I18nContextPtr myContext = boost::static_pointer_cast<I18nContext>(myComponent);
            if (myContext) {
                _myI18nContext = myContext;
            }
        }
    }

    std::vector<I18nContextPtr> 
    Widget::getI18nContexts() {
        std::vector<I18nContextPtr> myContexts;
        //XXX: does not work if method is const, why?
        WidgetPtr myCurrent =  boost::static_pointer_cast<Widget>(shared_from_this());
        while (myCurrent) {
            if (myCurrent->getI18nContext()) {
                myContexts.push_back(myCurrent->getI18nContext());
            }
            myCurrent = boost::dynamic_pointer_cast<Widget>(myCurrent->getParent());
        }
        return myContexts;
    }

    I18nItemPtr
    Widget::getI18nItemByName(const std::string & theName) {
        I18nItemPtr myI18nItem;
        std::vector<I18nContextPtr> myContexts = getI18nContexts();
        for (std::vector<I18nContextPtr>::iterator it = myContexts.begin(); it != myContexts.end(); ++it) {
            (*it)->setup();  //??? good idea? this avoids postrealize
            ComponentPtr myComponent = (*it)->getChildByName(theName);
            if (myComponent) {
                myI18nItem = boost::static_pointer_cast<I18nItem>(myComponent);
                if (myI18nItem) {
                    return myI18nItem;
                }
            }
        }
        return myI18nItem;
    }

    LANGUAGE 
    Widget::getLanguage() {
        std::vector<I18nContextPtr> myI18nContexts = getI18nContexts();
        if (myI18nContexts.size() > 0) {
            return myI18nContexts[0]->getLanguage();
        }
        return NO_LANGUAGE;
    }

    void 
    Widget::switchLanguage(LANGUAGE theLanguage) {
        std::vector<I18nContextPtr> myI18nContexts = getI18nContexts();
        for (std::vector<I18nContextPtr>::iterator it = myI18nContexts.begin(); it != myI18nContexts.end(); ++it) {
            (*it)->switchLanguage(theLanguage);
        }
    }
}
