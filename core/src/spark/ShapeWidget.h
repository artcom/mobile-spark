// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_ShapeWidget_
#define _included_mobile_spark_ShapeWidget_

#include "Widget.h"
#include <masl/Callback.h>
#include <mar/Shape.h>

namespace mar {
    class Material;
    typedef masl::Ptr<Material> MaterialPtr;
}

namespace spark {

    class ShapeWidget : public Widget {
        public:
            ShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ShapeWidget() = 0;
            
            virtual void prerender(MatrixStack& theCurrentMatrixStack);
            virtual void render(const matrix & theP) const;
            virtual void onPause();
            virtual void onResume();

            virtual bool AABB2Dcontains(const float x, const float y,
                                        const matrix & theProjectionMatrix) const;
            bool touches2DScreenCoords( mar::BoundingBox & theBB, const matrix & theProjectionMatrix) const;
    
            inline mar::ShapePtr getShape() const {return _myShape;};
            float getWorldZ() const;
            void setSize(const float theWidth, const float theHeight) { setSize(vector2(theWidth, theHeight));};
            void setSize(const vector2 & theSize);
            virtual const vector2 getSize();

            std::map<std::string, float> customShaderValues_;
            virtual std::string getAttributesAsString() const;

        protected:
            virtual mar::ShapePtr createCustomShape(const mar::MaterialPtr theMaterial);
            void makeMVPBB(mar::BoundingBox & theBB, const matrix & theProjectionMatrix) const;
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
