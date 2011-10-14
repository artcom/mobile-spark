#include "Element.h"
#include <masl/Logger.h>


namespace mar {
    /////////////////////////////////////////////////////////////Element
    Element::Element():
        #ifdef iOS
        vertexArrayObject_(0),
        #endif
        vertexBuffer_(0),
        indexBuffer_(0),
        stride_(VERTEX_SIZE)
    {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_POS_INDEX, VERTEX_POS_SIZE, VERTEX_POS_SIZE * sizeof(float)));
    }

    Element::~Element() {
        glDeleteBuffers(1, &vertexBuffer_);
        glDeleteBuffers(1, &indexBuffer_);
#ifdef iOS
        glDeleteVertexArraysOES(1, &vertexArrayObject_);
#endif
    }

    void
    Element::initGL() {
        material->initGL();
        createVertexBuffers();
    }

    void
    Element::createVertexBuffers() {

#ifdef iOS
        //Initialize the Vertex Array Object.
        glGenVertexArraysOES(1, &vertexArrayObject_);
        glBindVertexArrayOES(vertexArrayObject_);
#endif
        glGenBuffers(1, &vertexBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, (numVertices * stride_), vertexData_.get(), GL_STATIC_DRAW);

#ifdef ANDROID
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

#ifdef iOS
        int offset = 0;
        for (std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) {
            glVertexAttribPointer(it->get<0>(), it->get<1>(), GL_FLOAT, GL_FALSE, stride_, (GLvoid*)offset);
            glEnableVertexAttribArray(it->get<0>());
            offset += it->get<2>();
        }
#endif

        glGenBuffers(1, &indexBuffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numIndices * sizeof(GLushort)), indexDataVBO_.get(), GL_STATIC_DRAW);

#ifdef iOS
        glBindVertexArrayOES(0);
#elif ANDROID
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    }

    void
    Element::loadData(const matrix & theMatrix) {
        material->loadShader(theMatrix);
        if (!vertexBuffer_ || ! indexBuffer_) {
            createVertexBuffers();
            updateCompleteVertexBuffersContent();
        }
#ifdef ANDROID
        int offset = 0;
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        for (std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) {
            glVertexAttribPointer(it->get<0>(), it->get<1>(), GL_FLOAT, GL_FALSE, stride_, (GLvoid*)offset);
            glEnableVertexAttribArray(it->get<0>());
            offset += it->get<2>();
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
#endif
    }

    void
    Element::unloadData() const {
#ifdef ANDROID
        for (std::vector<boost::tuple<unsigned int, unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) {
            glDisableVertexAttribArray(it->get<0>());
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    }

    void
    Element::draw() const {
#ifdef iOS
        glBindVertexArrayOES(vertexArrayObject_);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, (void*)0);
        glBindVertexArrayOES(0);
#elif ANDROID
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
#endif
        checkGlError("glDrawElements");
    }


    void Element::updateCompleteVertexBuffersContent() {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glBufferSubData(GL_ARRAY_BUFFER,0,(numVertices * stride_), vertexData_.get());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /////////////////////////////////////////////////////////////ElementWithNormals
    ElementWithNormals::ElementWithNormals() : Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, VERTEX_NORMAL_SIZE * sizeof(float)));
        stride_ = NORMAL_VERTEX_SIZE;
    }

    ElementWithNormals::~ElementWithNormals() {
    }

    /////////////////////////////////////////////////////////////ElementWithTexture
    ElementWithTexture::ElementWithTexture() : Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, VERTEX_TEXCOORD0_SIZE * sizeof(float)));
        stride_ = TEXTURED_VERTEX_SIZE;
    }

    ElementWithTexture::~ElementWithTexture() {
    }

    /////////////////////////////////////////////////////////////ElementWithNormalsAndTexture
    ElementWithNormalsAndTexture::ElementWithNormalsAndTexture() : Element() {
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, VERTEX_NORMAL_SIZE * sizeof(float)));
        _myConfig.push_back(boost::tuple<unsigned int, unsigned int, unsigned int>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, VERTEX_TEXCOORD0_SIZE * sizeof(float)));
        stride_ = TEXTURED_NORMAL_VERTEX_SIZE;
    }

    ElementWithNormalsAndTexture::~ElementWithNormalsAndTexture() {
    }

}

