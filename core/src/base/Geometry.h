#ifndef _ac_mobile_asl_Material_h_included_
#define _ac_mobile_asl_Material_h_included_

#include <map>
#include <vector>
#include <string>
#include <GLES2/gl2.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include "MatrixStack.h"
#include "Material.h"

//vertex data size
#define VERTEX_POS_SIZE        3 //x, y, z
#define VERTEX_NORMAL_SIZE     3 //x, y, z
#define VERTEX_TEXCOORD0_SIZE  2  //s, t
#define NORMAL_VERTEX_SIZE (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE)
#define TEXTURED_VERTEX_SIZE (VERTEX_POS_SIZE + VERTEX_TEXCOORD0_SIZE)
#define TEXTURED_NORMAL_VERTEX_SIZE (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE + VERTEX_TEXCOORD0_SIZE)


namespace asl {

    class Element {
    public:
        unsigned int numVertices;
        boost::shared_array<float> vertexData;    //interleaved
        MaterialPtr material;

        virtual void loadData(const matrix & theMatrix) const;
        virtual void unloadData() const;
    };

    typedef boost::shared_ptr<Element> ElementPtr;

    class ElementWithNormals : public Element {
    public:
        virtual void loadData(const matrix & theMatrix) const;
        virtual void unloadData() const;
    };

    class ElementWithTexture : public Element {
    public: 
        virtual void loadData(const matrix & theMatrix) const;    
        virtual void unloadData() const;
    };

    class ElementWithNormalsAndTexture : public Element {
    public: 
        virtual void loadData(const matrix & theMatrix) const;    
        virtual void unloadData() const;
    };

    class Shape {
    public:
        std::vector<ElementPtr> elementList; 

        virtual void draw(const matrix & theMvp) const;
    };

    typedef boost::shared_ptr<Shape> ShapePtr;
};

#endif 
