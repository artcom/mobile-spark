#ifndef _ac_mobile_mar_Shape_h_included_
#define _ac_mobile_mar_Shape_h_included_

#include <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/Singleton.h>
#include <masl/MatrixStack.h>

#include "Element.h"


namespace mar {

    class Shape {
    public:
        Shape(const bool theTexturedFlag = false);
        virtual ~Shape();
        virtual void render(const matrix & theMvp) const;
        virtual void setDimensions(const float theWidth, const float theHeight) = 0;

        std::vector<ElementPtr> elementList; 
    protected:
        bool _myTextureFlag;   //XXX does not make any sense, texture is decided at element level
        size_t _myDataPerVertex;
    };
    typedef boost::shared_ptr<Shape> ShapePtr;


    class RectangleShape : public Shape {
    public:
        RectangleShape(const bool theTexturedFlag);
        virtual ~RectangleShape();
        virtual void setDimensions(const float theWidth, const float theHeight);
    private:
        virtual void setVertexData();
    };

    class ObjShape : public Shape {
    public:
        ObjShape();
        virtual ~ObjShape();
        virtual void setDimensions(const float theWidth, const float theHeight) {};
    };


    class ShapeFactory : public masl::Singleton<ShapeFactory> {
    public:
        ShapePtr createRectangle(const bool theTexturedFlag);
        ~ShapeFactory();        
        //ShapePtr createNinePatch();
        ShapePtr createObj(const std::string & theFile);
    };
};

#endif 
