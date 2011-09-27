#ifndef _included_mobile_spark_ShapeWidget_
#define _included_mobile_spark_ShapeWidget_

#include "Widget.h"
#include <mar/Shape.h>

namespace spark {

    class ShapeWidget : public Widget {
        public:
            ShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ShapeWidget() = 0;
            virtual void render(const matrix & theP) const;
            virtual void onResume();

            virtual bool AABB2Dcontains(const float x, const float y,
                                        const matrix & theProjectionMatrix) const;

            inline mar::ShapePtr getShape() const {return _myShape;};
            void setShape( mar::ShapePtr theShapePtr);
            float getWorldZ() const;
        protected:
            virtual void applyAlpha (const float theAlpha);
        private:
            mar::ShapePtr _myShape;
    };
    typedef masl::Ptr<ShapeWidget> ShapeWidgetPtr;
};

#endif
