#ifndef _ac_mobile_mar_Element_h_included_
#define _ac_mobile_mar_Element_h_included_

#include <map>
#include <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>

#include <masl/MatrixStack.h>

#include "Material.h"


namespace mar {

    //shader program handles
    const unsigned int VERTEX_POS_INDEX       = 0;
    const unsigned int VERTEX_TEXCOORD0_INDEX = 2;
    const unsigned int VERTEX_NORMAL_INDEX    = 1;

    //vertex data size
    const unsigned int VERTEX_POS_SIZE       = 3; //x, y, z
    const unsigned int VERTEX_NORMAL_SIZE    = 3; //x, y, z
    const unsigned int VERTEX_TEXCOORD0_SIZE = 2;  //s, t
    const unsigned int VERTEX_SIZE = VERTEX_POS_SIZE * sizeof(float);
    const unsigned int NORMAL_VERTEX_SIZE = ((VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE) * (sizeof(float)));
    const unsigned int TEXTURED_VERTEX_SIZE = ((VERTEX_POS_SIZE + VERTEX_TEXCOORD0_SIZE) * (sizeof(float)));
    const unsigned int TEXTURED_NORMAL_VERTEX_SIZE = ((VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE + VERTEX_TEXCOORD0_SIZE) * (sizeof(float)));
    
    enum {
        ATTRIB_VERTEX,
        NUM_ATTRIBUTES };
    
    typedef struct {
        float position[3];
    } vertexStruct;
    

    
    const vertexStruct vertices[] = {
        {{5.5, -5.5, 0}},
        {{5.5, 5.5, 0}},
        {{-5.5, 5.5, 0}},
        {{-5.5, -5.5, 0}}
    };
    
    const GLubyte indices[] = {
        3, 0, 2, 1
    };

    class Element {
    public:
        Element();
        virtual ~Element();

        virtual void loadData(const matrix & theMatrix) const;
        virtual void unloadData() const;
        virtual void createVertexBuffers();
        MaterialPtr material;
        unsigned int numVertices;
        boost::shared_array<float> vertexData;    //interleaved
        
        //for VBO's
        boost::shared_array<GLushort> indexDataVBO;
        
        boost::shared_array<float> testVertices;
        boost::shared_array<GLushort> testIndices;
        GLuint vertexBuffer;
        GLuint indexBuffer;
        
    protected:
        std::vector<std::pair<unsigned int, unsigned int> > _myConfig;
        unsigned int _myStride;
    };

    typedef boost::shared_ptr<Element> ElementPtr;

    class ElementWithNormals : public Element {
    public:
        ElementWithNormals();
        virtual ~ElementWithNormals();
    };

    class ElementWithTexture : public Element {
    public: 
        ElementWithTexture();
        virtual ~ElementWithTexture();
    };

    class ElementWithNormalsAndTexture : public Element {
    public: 
        ElementWithNormalsAndTexture();
        virtual ~ElementWithNormalsAndTexture();
    };
};

#endif 
