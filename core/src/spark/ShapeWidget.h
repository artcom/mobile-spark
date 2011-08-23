#ifndef _included_mobile_spark_ShapeWidget_
#define _included_mobile_spark_ShapeWidget_

#include "Widget.h"
#include <mar/Geometry.h>

namespace spark {

    class ShapeWidget : public Widget {
        public: 
            ShapeWidget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
            virtual ~ShapeWidget();
            virtual void renderWithLocalMatrix(MatrixStack& theCurrentMatrixStack) const;
            
            mar::ShapePtr _myShape;            
        private:
    };
    typedef boost::shared_ptr<ShapeWidget> ShapeWidgetPtr;
};

#endif
