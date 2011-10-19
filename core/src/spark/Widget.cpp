
#include "Widget.h"
#include "I18nContext.h"
#include "BaseApp.h"

#include <masl/numeric_functions.h>
#include <cml/mathlib/matrix_transform.h>
#include <cml/mathlib/matrix_rotation.h>
#include <cml/mathlib/matrix_translation.h>

namespace spark {
    Widget::Widget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Container(theApp, theXMLNode),
          _myDirtyFlag(true),
          _x(_myXMLNode->getAttributeAs<float>("x", 0)),
          _y(_myXMLNode->getAttributeAs<float>("y", 0)),
          _z(_myXMLNode->getAttributeAs<float>("z", 0)),
          _scaleX(_myXMLNode->getAttributeAs<float>("scaleX",1)),
          _scaleY(_myXMLNode->getAttributeAs<float>("scaleY",1)),
          _scaleZ(_myXMLNode->getAttributeAs<float>("scaleZ",1)),
          _rotationX(_myXMLNode->getAttributeAs<float>("rotationX", 0)),
          _rotationY(_myXMLNode->getAttributeAs<float>("rotationY", 0)),
          _rotationZ(_myXMLNode->getAttributeAs<float>("rotationZ", 0)),
          _alpha(_myXMLNode->getAttributeAs<float>("alpha", 1.0)),
          _actualAlpha(_alpha),
          _visible(_myXMLNode->getAttributeAs<bool>("visible", true)),
          _sensible(_myXMLNode->getAttributeAs<bool>("sensible", true))
          
    {
        updateMatrix();
        setI18nContextIfAvailable();
    }

    Widget::~Widget() {
    }

    void
    Widget::realize() {
        Container::realize();
        setAlpha(_alpha);
    }

    void
    Widget::updateMatrix() {

        matrix myLocalMatrix;
        myLocalMatrix.identity();
        
        matrix myScalingMatrix;
        myScalingMatrix.identity();
        cml::matrix_scale(myScalingMatrix, _scaleX, _scaleY, _scaleZ);

        matrix myRotationMatrix;
        myRotationMatrix.identity();
        cml::matrix_rotation_euler(myRotationMatrix, _rotationX, _rotationY, _rotationZ,  cml::euler_order_xyz);
                
        matrix myTranslationMatrix;
        myTranslationMatrix.identity();
        cml::matrix_translation(myTranslationMatrix, _x, _y, _z);

        myLocalMatrix *= myTranslationMatrix;
        myLocalMatrix *= myRotationMatrix;
        myLocalMatrix *= myScalingMatrix;
        
        _myLocalMatrix = myLocalMatrix; 

    }

    //TODO: use visitor?
    void
    Widget::prerender(MatrixStack& theCurrentMatrixStack) {
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

    bool
    Widget::isRendered() const {
        if (!_visible) {
            return false;
        } else {
            if (getParent().lock()) {
                return getParent().lock()->isRendered();
            } else {
                return _visible;
            }
        }
    }

    void
    Widget::render(const matrix & theProjectionMatrix) const {
    }

    float
    Widget::getParentAlpha() const {
        float myParentAlpha = 1.0;
        WidgetPtr myParent = boost::dynamic_pointer_cast<Widget>(getParent().lock());
        if (myParent) {
            myParentAlpha = myParent->getActualAlpha();
        }
        return myParentAlpha;
    }

    void
    Widget::propagateAlpha() {
        _actualAlpha = getParentAlpha() * _alpha;
        AC_TRACE << "Widget::propagateAlpha() " << getName();
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            WidgetPtr myChild = boost::dynamic_pointer_cast<Widget>(*it);
            if (myChild) {
                AC_TRACE << "Widget::propagateAlpha() ->child: " << *myChild;
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
    Widget::getI18nContexts() const {
        std::vector<I18nContextPtr> myContexts;
        masl::Ptr<const Component> myCurrent =  boost::static_pointer_cast<const Component>(shared_from_this());
        while (myCurrent) {
            masl::Ptr<const Widget> myWidget = boost::dynamic_pointer_cast<const Widget>(myCurrent);
            if (myWidget && myWidget->getI18nContext()) {
                myContexts.push_back(myWidget->getI18nContext());
            }
            myCurrent = myCurrent->getParent().lock();
        }
        return myContexts;
    }

    I18nItemPtr
    Widget::getI18nItemByName(const std::string & theName) const {
        I18nItemPtr myI18nItem;
        std::vector<I18nContextPtr> myContexts = getI18nContexts();
        for (std::vector<I18nContextPtr>::iterator it = myContexts.begin(); it != myContexts.end(); ++it) {
            ComponentPtr myComponent = (*it)->getChildByName(theName);
            if (myComponent && myComponent->getType() == I18nItem::SPARK_TYPE) {
                return boost::static_pointer_cast<I18nItem>(myComponent);
            }
        }
        return myI18nItem;
    }

    LANGUAGE 
    Widget::getLanguage() const {
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

    std::string 
    Widget::getAttributesAsString() const {
        return Container::getAttributesAsString() + 
            " visible=\""+masl::as_string(_visible)+"\"" + " sensible=\""+masl::as_string(_sensible)+"\""
            " alpha=\""+masl::as_string(_alpha)+"\"" + " actualAlpha=\""+masl::as_string(_actualAlpha)+"\"" 
            " dirtyFlag=\""+masl::as_string(_myDirtyFlag)+"\""
            " x=\""+masl::as_string(_x)+"\"" + " y=\""+masl::as_string(_y)+"\"" + " z=\""+masl::as_string(_z)+"\"" 
            " scaleX=\""+masl::as_string(_scaleX)+"\"" + " scaleY=\""+masl::as_string(_scaleY)+"\"" 
                " scaleZ=\""+masl::as_string(_scaleZ)+"\""
            " rotationX=\""+masl::as_string(_rotationX)+"\"" + " rotationY=\""+masl::as_string(_rotationY)+"\"" 
                " rotationZ=\""+masl::as_string(_rotationZ)+"\"";
    }
}
