#ifndef _included_mobile_spark_ShapeWidget_
#define _included_mobile_spark_ShapeWidget_

#include "Widget.h"
#include <mar/Shape.h>

namespace spark {

    class ShapeWidget : public Widget {
        public: 
            ShapeWidget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
            virtual ~ShapeWidget() = 0;
            virtual void renderWithLocalMatrix(MatrixStack& theCurrentMatrixStack) const;
            virtual void onResume() const;        
            mar::ShapePtr getShape();            
            void setShape( mar::ShapePtr theShapePtr);            
        private:
            mar::ShapePtr _myShape;            
    };
    typedef boost::shared_ptr<ShapeWidget> ShapeWidgetPtr;
};

#endif
