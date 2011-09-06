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

    class Shape {
    public:
        Shape(const bool theTexturedFlag = false);
        virtual ~Shape();
        virtual void render(const matrix & theMvp) const;
        virtual void initGL();
        virtual void setDimensions(const float theWidth, const float theHeight) = 0;
        BoundingBox & getBoundingBox() { return _myBoundingBox;};

        std::vector<ElementPtr> elementList; 
    protected:
        bool _myTextureFlag;   //XXX does not make any sense, texture is decided at element level
        size_t _myDataPerVertex;
        BoundingBox _myBoundingBox;
    };
    typedef boost::shared_ptr<Shape> ShapePtr;


    class RectangleShape : public Shape {
    public:
        RectangleShape(const bool theTexturedFlag, const std::string & theTextureSrc = "");
        virtual ~RectangleShape();
        virtual void setDimensions(const float theWidth, const float theHeight);
    private:
        virtual void setVertexData();
    };

    class NinePatchShape : public Shape {
    public:
        NinePatchShape(const std::string & theTextureSrc, const float theLeftEdge, 
                const float theTopEdge, const float theRightEdge, const float theBottomEdge);
        virtual ~NinePatchShape();
        virtual void setDimensions(const float theWidth, const float theHeight);
    private:
        virtual void setVertexData();
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
        ShapePtr createRectangle(const bool theTexturedFlag, const std::string & theTextureSrc = "");
        ~ShapeFactory();        
        ShapePtr createNinePatch(const std::string & theTextureSrc, const float theLeftEdge, 
                const float theTopEdge, const float theRightEdge, const float theBottomEdge);
        ShapePtr createObj(const std::string & theFile);
    };

};

#endif 
