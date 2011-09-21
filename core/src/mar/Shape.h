#ifndef _ac_mobile_mar_Shape_h_included_
#define _ac_mobile_mar_Shape_h_included_

#include <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/Singleton.h>
#include <masl/MatrixStack.h>

#include "Element.h"


namespace mar {

    struct BoundingBox {
        vector4 min;
        vector4 max;
    };

    struct Transparency {
        enum type {
            TRANSPARENT,
            OPAQUE,
            UNKNOWN
        };
    };

    class Shape {
    public:
        Shape(const bool theTexturedFlag = false);
        virtual ~Shape();
        virtual void render(const matrix & theMvp) const;
        virtual void initGL();
        virtual void setDimensions(const float theWidth, const float theHeight) = 0;
        virtual void setTexCoords(vector2 theUV0, vector2 theUV1, vector2 theUV2, vector2 theUV3) {}
        const BoundingBox & getBoundingBox() const { return _myBoundingBox;};
        void setBoundingBox(const vector4 theMin, const vector4 theMax);
        BoundingBox & getBoundingBox() { return _myBoundingBox;};
        bool isTransparent();

        void setAlpha(const float theAlpha);
        std::vector<ElementPtr> elementList;
    protected:
        bool _myTextureFlag;   //XXX does not make any sense, texture is decided at element level
        size_t _myDataPerVertex;
        BoundingBox _myBoundingBox;
    private:
        Transparency::type _myTransparency;
    };
    typedef boost::shared_ptr<Shape> ShapePtr;


    class RectangleShape : public Shape {
    public:
        RectangleShape(const bool theTexturedFlag, const float theWidth = 0, const float theHeight = 0,
                       const std::string & theTextureSrc = "");
        virtual ~RectangleShape();
        void setTexCoords(vector2 theUV0, vector2 theUV1, vector2 theUV2, vector2 theUV3);
        virtual void setDimensions(const float theWidth, const float theHeight);
    private:
        virtual void setVertexData();
        float width_;
        float height_;
    };

    class NinePatchShape : public Shape {
    public:
        NinePatchShape(const std::string & theTextureSrc, const float theLeftEdge,
                const float theTopEdge, const float theRightEdge, const float theBottomEdge,
                const float theWidth = 0, const float theHeight = 0);
        virtual ~NinePatchShape();
        virtual void setDimensions(const float theWidth, const float theHeight);
    private:
        virtual void setVertexData();
        float width_;
        float height_;
        float leftEdge_;
        float topEdge_;
        float rightEdge_;
        float bottomEdge_;
        float imageWidth_;
        float imageHeight_;
    };

    class ObjShape : public Shape {
    public:
        ObjShape();
        virtual ~ObjShape();
        virtual void setDimensions(const float theWidth, const float theHeight) {};
        void setBoundingBox(const vector4 theMin, const vector4 theMax);
    };
    typedef boost::shared_ptr<ObjShape> ObjShapePtr;


    class ShapeFactory : public masl::Singleton<ShapeFactory> {
    public:
        ShapePtr createRectangle(const bool theTexturedFlag, const float theWidth = 0, const float theHeight = 0, const std::string & theTextureSrc = "");
        ~ShapeFactory();
        ShapePtr createNinePatch(const std::string & theTextureSrc, const float theLeftEdge = 0,
                const float theTopEdge = 0, const float theRightEdge = 0, const float theBottomEdge = 0,
                const float theWidth = 0, const float theHeight = 0);
        ShapePtr createObj(const std::string & theFile);
    };

};

#endif
