#ifndef _ac_mobile_mar_Element_h_included_
#define _ac_mobile_mar_Element_h_included_

#include <map>
#include <vector>
#include <string>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/tuple/tuple.hpp>

#include <masl/Ptr.h>
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

    class Element {
    public:
        Element();
        virtual ~Element();

        void resetGL();
        void draw() const;
        void loadData(const matrix & theMatrix);
        void unloadData() const;
        void createVertexBuffers();
        void updateCompleteVertexBuffersContent();
        MaterialPtr material_;
        unsigned int numVertices_;
        unsigned int numIndices_;

        boost::shared_array<float> vertexData_;    //interleaved
        boost::shared_array<GLushort> indexDataVBO_;

    protected:
        void deleteVertexBuffers();
        std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> > _myConfig;
        #ifdef iOS
        GLuint vertexArrayObject_;
        #endif

        GLuint vertexBuffer_;
        GLuint indexBuffer_;
        unsigned int stride_;
    };

    typedef masl::Ptr<Element> ElementPtr;

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
