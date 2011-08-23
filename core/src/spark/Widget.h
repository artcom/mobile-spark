#ifndef _included_mobile_spark_Widget_
#define _included_mobile_spark_Widget_

#include <string>

#include <masl/Logger.h>
#include <masl/MatrixStack.h>
#include <animation/PropertyAnimation.h>
#include <animation/Callback.h>

#include "Container.h"

namespace spark {

    class Widget : public Container {
    public: 
        Widget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Widget();
        virtual void render(MatrixStack& theCurrentMatrixStack, matrix theProjectionMatrix) const;

        void setX(const float theX) { _x = theX; updateMatrix();};
        void setY(const float theY) { _y = theY; updateMatrix();};
        void setZ(const float theZ) { _z = theZ; updateMatrix();};
        void setScaleX(const float theScaleX) { _scaleX = theScaleX; updateMatrix();};
        void setScaleY(const float theScaleY) { _scaleY = theScaleY; updateMatrix();};
        void setScaleZ(const float theScaleZ) { _scaleZ = theScaleZ; updateMatrix();};
        void setRotationX(const float theRotationX) { _rotationX = theRotationX; updateMatrix();};
        void setRotationY(const float theRotationY) { _rotationY = theRotationY; updateMatrix();};
        void setRotationZ(const float theRotationZ) { _rotationZ = theRotationZ; updateMatrix();};
        void setAlpha(const float theAlpha)  { _alpha = theAlpha; };

        void test() {
            AC_PRINT << "test callback";
        }
        MatrixStack _myLocalMatrixStack; //scale, roation and translation of this node
        
    protected:
    private:
        float _x,_y,_z;
        float _scaleX, _scaleY, _scaleZ;
        float _rotationX, _rotationY, _rotationZ;
        float _alpha;
        bool _visible;

        void updateMatrix();
    };

    typedef boost::shared_ptr<Widget> WidgetPtr;

    //animations
    typedef void (Widget::* WidgetPropertySetterFunction)(float);
    typedef void (Widget::* WidgetMemberFunction)();
    typedef animation::PropertyAnimation<WidgetPtr, WidgetPropertySetterFunction> WidgetPropertyAnimation;
    typedef boost::shared_ptr<WidgetPropertyAnimation>  WidgetPropertyAnimationPtr;

    class WidgetCallback : public animation::Callback {
    public:
        WidgetCallback(WidgetPtr theWidget, WidgetMemberFunction theFunctionPtr): 
            animation::Callback(),
            _myWidgetPtr(theWidget),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~WidgetCallback() {};

        virtual void execute() const {
            (_myWidgetPtr.get()->*_myFunctionPointer)();
        };
    private:
        WidgetPtr _myWidgetPtr;
        WidgetMemberFunction _myFunctionPointer;
    };
    typedef boost::shared_ptr<WidgetCallback> WidgetCallbackPtr;
};
#endif
