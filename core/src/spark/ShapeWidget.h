#ifndef _included_mobile_spark_ShapeWidget_
#define _included_mobile_spark_ShapeWidget_

#include "Widget.h"
#include <masl/Callback.h>

namespace mar {
    class Shape;
    typedef masl::Ptr<Shape> ShapePtr;
}

namespace spark {

    class ShapeWidget : public Widget {
        public:
            ShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ShapeWidget() = 0;
            
            virtual void prerender(MatrixStack& theCurrentMatrixStack);
            virtual void render(const matrix & theP) const;
            virtual void onResume();

            virtual bool AABB2Dcontains(const float x, const float y,
                                        const matrix & theProjectionMatrix) const;

            inline mar::ShapePtr getShape() const {return _myShape;};
            float getWorldZ() const;
            void setSize(const float theWidth, const float theHeight) { setSize(vector2(theWidth, theHeight));};
            void setSize(const vector2 & theSize);

            std::map<std::string, float> customShaderValues_;
            virtual std::string getAttributesAsString() const;

        protected:
            mar::ShapePtr _myShape;
            virtual void propagateAlpha();
            std::string vertexShader_;
            std::string fragmentShader_;
        private:
            enum ORIGIN_MODE {
                NO_ORIGIN,
                CENTER_ORIGIN,
                EXPLICIT_ORIGIN
            };
            vector2 _myOrigin;
            ORIGIN_MODE _myOriginMode;
                
    };
    typedef masl::Ptr<ShapeWidget> ShapeWidgetPtr;
};

#endif
