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
};
#endif
