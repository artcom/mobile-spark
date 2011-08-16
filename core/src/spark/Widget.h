#ifndef _included_mobile_spark_Widget_
#define _included_mobile_spark_Widget_

#include <string>

#include <masl/MatrixStack.h>

#include "Container.h"

namespace spark {

    class Widget : public Container {
    public: 
        Widget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Widget();
        virtual void render() const;

        void setX(const float theX) { _x = theX; };
        void setY(const float theY) { _y = theY; };
        void setZ(const float theZ) { _z = theZ; };
        void setScaleX(const float theScaleX) { _scaleX = theScaleX; };
        void setScaleY(const float theScaleY) { _scaleY = theScaleY; };
        void setScaleZ(const float theScaleZ) { _scaleZ = theScaleZ; };
        void setRotationX(const float theRotationX) { _rotationX = theRotationX; };
        void setRotationY(const float theRotationY) { _rotationY = theRotationY; };
        void setRotationZ(const float theRotationZ) { _rotationZ = theRotationZ; };
        void setAlpha(const float theAlpha)  { _alpha = theAlpha; };
        
    protected:
        MatrixStack _myLocalMatrixStack; //scale, roation and translation of this node
    private:
        float _x,_y,_z;
        float _scaleX, _scaleY, _scaleZ;
        float _rotationX, _rotationY, _rotationZ;
        float _alpha;
        bool _visible;
    };

    typedef boost::shared_ptr<Widget> WidgetPtr;

    static void setTest(const float theTest) {};
};
#endif
