// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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

    typedef boost::shared_array<float> VertexData;
    typedef boost::shared_array<GLushort> IndexData;

    class Element {
    public:
        Element();
        virtual ~Element();

        void resetGL();
        void draw();
        void loadData(const matrix & theMatrix);
        void unloadData() const;
        void createVertexBuffers();
        std::string getAttributesAsString() const;
        VertexData & getVertexData() { dirtyFlag_ = true; return vertexData_; };
        IndexData & getIndexData() { dirtyFlag_ = true; return indexData_; };
        void setVertexData(const VertexData & theData) { vertexData_ = theData; dirtyFlag_ = true;};
        void setIndexData(const IndexData & theData) { indexData_ = theData; dirtyFlag_ = true;};

        MaterialPtr material_;
        unsigned int numVertices_;
        unsigned int numIndices_;


    protected:
        void updateCompleteVertexBuffersContent();
        void deleteVertexBuffers();

        bool dirtyFlag_;
        VertexData vertexData_;    //interleaved
        IndexData indexData_;
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
