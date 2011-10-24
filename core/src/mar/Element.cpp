// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Element.h"
#include <masl/Logger.h>
#include <mar/openGL_functions.h>


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
        deleteVertexBuffers();
    }

    //ANDROID ONLY: gl context is lost, so reset all buffers to get new ones
    void
    Element::resetGL() {
        material_->resetGL();
        vertexBuffer_ = 0;
        indexBuffer_ = 0;
#ifdef iOS
        vertexArrayObject_ = 0;
#endif
    }

    void
    Element::deleteVertexBuffers() {
        if (vertexBuffer_) {
            glDeleteBuffers(1, &vertexBuffer_);
            ASSERT_GL("glDeleteBuffers vertexBuffer", PLUS_FILE_LINE);
            vertexBuffer_ = 0;
        }
        if (indexBuffer_) {
            glDeleteBuffers(1, &indexBuffer_);
            ASSERT_GL("glDeleteBuffers indexBuffer", PLUS_FILE_LINE);
            indexBuffer_ = 0;
        }
#ifdef iOS
        if (vertexArrayObject_) {
            glDeleteVertexArraysOES(1, &vertexArrayObject_);
            vertexArrayObject_ = 0;
        }
#endif
        ASSERT_GL("Element::deleteVertexBuffers", PLUS_FILE_LINE);
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
        glBufferData(GL_ARRAY_BUFFER, (numVertices_ * stride_), vertexData_.get(), GL_STATIC_DRAW);

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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numIndices_ * sizeof(GLushort)), indexDataVBO_.get(), GL_STATIC_DRAW);

#ifdef iOS
        glBindVertexArrayOES(0);
#elif ANDROID
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
        ASSERT_GL("Element::createVertexBuffers", PLUS_FILE_LINE);
    }

    void
    Element::loadData(const matrix & theMatrix) {
        material_->loadShader(theMatrix);
        if (!vertexBuffer_ || ! indexBuffer_) {
            createVertexBuffers();
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
        ASSERT_GL("Element::loadData", PLUS_FILE_LINE);
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
        material_->unloadShader();
        ASSERT_GL("Element::unloadData", PLUS_FILE_LINE);
    }

    void
    Element::draw() const {
#ifdef iOS
        glBindVertexArrayOES(vertexArrayObject_);
        glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_SHORT, (void*)0);
        glBindVertexArrayOES(0);
#elif ANDROID
        glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_SHORT, 0);
#endif
        ASSERT_GL("Element::draw", PLUS_FILE_LINE);
    }

    //XXX: this should not be called from outside, if vertexdata is outdated they should automatically updated
    void Element::updateCompleteVertexBuffersContent() {
        if (vertexBuffer_) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
            glBufferSubData(GL_ARRAY_BUFFER,0,(numVertices_ * stride_), vertexData_.get());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            ASSERT_GL("Element::updateCompleteVertexBuffersContent", PLUS_FILE_LINE);
        }
    }

    std::string
    Element::getAttributesAsString() const {
        return "material={"+(material_?material_->getAttributesAsString():"")+"}";
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

