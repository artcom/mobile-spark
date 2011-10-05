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
        Shape(const bool theTexturedFlag = false);
        virtual ~Shape();
        void render(const matrix & theMvp) const;
        void updateHandles(const std::map<std::string, float> & theShaderValues);
        void initGL();
        virtual void setDimensions(const float theWidth, const float theHeight) = 0;
        virtual void setTexCoords(const vector2 & theUV0, const vector2 & theUV1, const vector2 & theUV2, const vector2 & theUV3) {}
        const BoundingBox & getBoundingBox() const { return _myBoundingBox;};
        void setBoundingBox(const vector4 & theMin, const vector4 & theMax);
        bool isTransparent();

        void setAlpha(const float theAlpha);
        std::vector<ElementPtr> elementList;
    protected:
        bool _myTextureFlag;   //XXX does not make any sense, texture is decided at element level
        size_t _myDataPerVertex;
        BoundingBox _myBoundingBox;
    private:
    };
    typedef masl::Ptr<Shape> ShapePtr;


    class RectangleShape : public Shape {
    public:
        RectangleShape(const bool theTexturedFlag, const float theWidth = 0, const float theHeight = 0,
                       const std::string & theVertexShader = "", const std::string & theFragmentShader = "",
                       const std::vector<std::string> & theCustomHandles = std::vector<std::string>(),
                       const std::string & theTextureSrc = "");
        virtual ~RectangleShape();
        virtual void setTexCoords(const vector2 & theUV0, const vector2 & theUV1, const vector2 & theUV2, const vector2 & theUV3);
        virtual void setDimensions(const float theWidth, const float theHeight);
    private:
        void setVertexData();
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
        void setVertexData();
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
    };
    typedef masl::Ptr<ObjShape> ObjShapePtr;


    class ShapeFactory : public masl::Singleton<ShapeFactory> {
    public:
        ShapePtr createRectangle(const bool theTexturedFlag, const float theWidth = 0, const float theHeight = 0, 
                                 const std::string & theVertexShader = "" , const std::string & theFragmentShader = "",
                                 const std::vector<std::string> & theCustomHandles = std::vector<std::string>(),
                                 const std::string & theTextureSrc = "");
        ~ShapeFactory();
        ShapePtr createNinePatch(const std::string & theTextureSrc, const float theLeftEdge = 0,
                const float theTopEdge = 0, const float theRightEdge = 0, const float theBottomEdge = 0,
                const float theWidth = 0, const float theHeight = 0);
        ShapePtr createObj(const std::string & theFile);
    };

};

#endif
