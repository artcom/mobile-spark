#include "Element.h"
#include <masl/Logger.h>


namespace mar {
    /////////////////////////////////////////////////////////////Element
    Element::Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_POS_INDEX, VERTEX_POS_SIZE, VERTEX_POS_SIZE * sizeof(float)));
        _myStride = VERTEX_SIZE;
    }

    Element::~Element() {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
#if __APPLE__
        glDeleteVertexArraysOES(1, &vertexArrayObject);
#endif
    }

#if __APPLE__
    void Element::loadData(const matrix & theMatrix) const {

    }

    void Element::unloadData() const {

    }

    void Element::createVertexBuffers() {

        //Initialize the Vertex Array Object.
        glGenVertexArraysOES(1, &vertexArrayObject);
        glBindVertexArrayOES(vertexArrayObject);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (numVertices * _myStride), vertexData_.get(), GL_STATIC_DRAW);

        int offset = 0;
        for (std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) {
            glVertexAttribPointer(it->get<0>(), it->get<1>(), GL_FLOAT, GL_FALSE, _myStride, (GLvoid*)offset);
            glEnableVertexAttribArray(it->get<0>());
            offset += it->get<2>();
        }

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numIndices * sizeof(GLushort)), indexDataVBO_.get(), GL_STATIC_DRAW);

        glBindVertexArrayOES(0);
    }
#endif
#ifdef __ANDROID__
    void Element::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);
        int offset = 0;
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        for (std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) {
            glVertexAttribPointer(it->get<0>(), it->get<1>(), GL_FLOAT, GL_FALSE, _myStride, (GLvoid*)offset);
            glEnableVertexAttribArray(it->get<0>());
            offset += it->get<2>();
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    }

    void Element::unloadData() const {
        for (std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) {
            glDisableVertexAttribArray(it->get<0>());
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Element::createVertexBuffers() {
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (numVertices * _myStride), vertexData_.get(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numIndices * sizeof(GLushort)), indexDataVBO_.get(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#endif


    void Element::updateCompleteVertexBuffersContent() {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER,0,(numVertices * _myStride), vertexData_.get());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /////////////////////////////////////////////////////////////ElementWithNormals
    ElementWithNormals::ElementWithNormals() : Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, VERTEX_NORMAL_SIZE * sizeof(float)));
        _myStride = NORMAL_VERTEX_SIZE;
    }

    ElementWithNormals::~ElementWithNormals() {
    }

    /////////////////////////////////////////////////////////////ElementWithTexture
    ElementWithTexture::ElementWithTexture() : Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, VERTEX_TEXCOORD0_SIZE * sizeof(float)));
        _myStride = TEXTURED_VERTEX_SIZE;
    }

    ElementWithTexture::~ElementWithTexture() {
    }

    /////////////////////////////////////////////////////////////ElementWithNormalsAndTexture
    ElementWithNormalsAndTexture::ElementWithNormalsAndTexture() : Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, VERTEX_NORMAL_SIZE * sizeof(float)));
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, VERTEX_TEXCOORD0_SIZE * sizeof(float)));
        _myStride = TEXTURED_NORMAL_VERTEX_SIZE;
    }

    ElementWithNormalsAndTexture::~ElementWithNormalsAndTexture() {
    }

}

