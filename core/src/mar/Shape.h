#ifndef _ac_mobile_mar_Shape_h_included_
#define _ac_mobile_mar_Shape_h_included_

#include <vector>
#include <string>

#include <masl/Ptr.h>
#include <masl/Singleton.h>
#include <masl/MatrixStack.h>

#include "Element.h"


namespace mar {

    struct BoundingBox {
        vector4 min;
        vector4 max;
    };

    class Shape {
    public:
        Shape(const float theWidth = 0, const float theHeight = 0);
        virtual ~Shape();
        void render(const matrix & theMvp) const;
        void updateCustomHandles(const std::map<std::string, float> & theShaderValues);
        void initGL();
        virtual void setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) = 0;
        virtual void setTexCoords(const vector2 & theUV0, const vector2 & theUV1, const vector2 & theUV2, const vector2 & theUV3) {};
        const BoundingBox & getBoundingBox() const { return boundingBox_;};
        void setBoundingBox(const vector4 & theMin, const vector4 & theMax);
        bool isTransparent() const;

        float getWidth() const { return upperRight_[0] - lowerLeft_[0];};
        float getHeight() const { return upperRight_[1] - lowerLeft_[1];};
        void setAlpha(const float theAlpha);

        std::vector<ElementPtr> elementList_;
    protected:
        size_t dataPerVertex_;
        BoundingBox boundingBox_;
        vector2 lowerLeft_;
        vector2 upperRight_;
    private:
    };
    typedef masl::Ptr<Shape> ShapePtr;


    class RectangleShape : public Shape {
    public:
        RectangleShape(const MaterialPtr theMaterial, const float theWidth = 0, const float theHeight = 0);
        virtual ~RectangleShape();
        virtual void setTexCoords(const vector2 & theUV0, const vector2 & theUV1, const vector2 & theUV2, const vector2 & theUV3);
        virtual void setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner);
    private:
        void setVertexData();
    };
    typedef masl::Ptr<RectangleShape> RectangleShapePtr;

    class NinePatchShape : public Shape {
    public:
        NinePatchShape(const MaterialPtr theMaterial, const float theWidth = 0, const float theHeight = 0);
        virtual ~NinePatchShape();
        virtual void setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner);
        void setEdges(const float theLeftEdge, const float theTopEdge,
                      const float theRightEdge, const float theBottomEdge);
    private:
        void setVertexData();
        float edgeLeft_;
        float edgeTop_;
        float edgeRight_;
        float edgeBottom_;
        float imageWidth_;
        float imageHeight_;
    };
    typedef masl::Ptr<NinePatchShape> NinePatchShapePtr;

    class ObjShape : public Shape {
    public:
        ObjShape();
        virtual ~ObjShape();
        virtual void setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {};
    };
    typedef masl::Ptr<ObjShape> ObjShapePtr;

};

#endif
