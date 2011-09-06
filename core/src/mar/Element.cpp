#include "Element.h"
#include <masl/Logger.h>


namespace mar {
    /////////////////////////////////////////////////////////////Element
    Element::Element() {
        _myConfig.push_back(std::pair<unsigned int, unsigned int>(VERTEX_POS_INDEX, VERTEX_POS_SIZE));
        _myStride = VERTEX_SIZE;
        testVertices = boost::shared_array<float>(new float[12]);
        testVertices[0] = 5.5;
        testVertices[1] = -5.5;
        testVertices[2] = 0.0;
        
        testVertices[3] = 5.5;
        testVertices[4] = 5.5;
        testVertices[5] = 0.0;
        
        testVertices[6] = -5.5;
        testVertices[7] = 5.5;
        testVertices[8] = 0.0;
        
        testVertices[9] = -5.5;
        testVertices[10] = -5.5;
        testVertices[11] = 0.0;
        
        testIndices = boost::shared_array<GLushort>(new GLushort[4]);
        testIndices[0] = 3;
        testIndices[1] = 0;
        testIndices[2] = 2;
        testIndices[3] = 1;

    }

    Element::~Element() {
    }

    void Element::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);
        int offset = 0;
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) { 
            glVertexAttribPointer(it->first, it->second, GL_FLOAT, GL_FALSE, _myStride, (GLvoid*)offset);
            glEnableVertexAttribArray(it->first);
            offset += it->second;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    }

    void Element::unloadData() const {
        for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) { 
            //glDisableVertexAttribArray(it->first);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
    }
    
    void Element::createVertexBuffers() {
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (numVertices * _myStride), vertexData.get(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        AC_PRINT << "_myStride: " << _myStride;
        AC_PRINT << "sizeof vertices: " << (numVertices * _myStride);
        
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numVertices * sizeof(GLushort)), indexDataVBO.get(), GL_STATIC_DRAW); 
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        AC_PRINT << "sizeof indices: " <<   (numVertices * sizeof(GLushort));

        int offset = 0;
        for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) { 
            AC_PRINT << "ATRIBUTE " << it->first;
            AC_PRINT << "offset: " << (GLvoid*)offset;
            offset += it->second;

        }

    }

    /////////////////////////////////////////////////////////////ElementWithNormals
    ElementWithNormals::ElementWithNormals() : Element() {
        _myConfig.push_back(std::pair<unsigned int, unsigned int>(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE));
        _myStride = NORMAL_VERTEX_SIZE;
    }

    ElementWithNormals::~ElementWithNormals() {
    }

    /////////////////////////////////////////////////////////////ElementWithTexture
    ElementWithTexture::ElementWithTexture() : Element() {
        _myConfig.push_back(std::pair<unsigned int, unsigned int>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE));
        _myStride = TEXTURED_VERTEX_SIZE;
    }

    ElementWithTexture::~ElementWithTexture() {
    }

    /////////////////////////////////////////////////////////////ElementWithNormalsAndTexture
    ElementWithNormalsAndTexture::ElementWithNormalsAndTexture() : Element() {
        _myConfig.push_back(std::pair<unsigned int, unsigned int>(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE));
        _myConfig.push_back(std::pair<unsigned int, unsigned int>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE));
        _myStride = TEXTURED_NORMAL_VERTEX_SIZE;
    }

    ElementWithNormalsAndTexture::~ElementWithNormalsAndTexture() {
    }

}

