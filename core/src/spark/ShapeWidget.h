#ifndef _included_mobile_spark_ShapeWidget_
#define _included_mobile_spark_ShapeWidget_

#include "Widget.h"
#include <mar/Shape.h>

namespace spark {

    class ShapeWidget : public Widget {
        public: 
            ShapeWidget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
            virtual ~ShapeWidget() = 0;
            virtual void render(const matrix & theP) const;
            virtual void onResume();
            
            virtual bool AABB2Dcontains(const float x, const float y,
                                        const matrix theProjectionMatrix) const;

            mar::ShapePtr getShape();            
            void setShape( mar::ShapePtr theShapePtr);            
            float getWorldZ() const;
        private:
            mar::ShapePtr _myShape;            
    };
    typedef boost::shared_ptr<ShapeWidget> ShapeWidgetPtr;
};

#endif
