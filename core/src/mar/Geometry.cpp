#include "Geometry.h"

#include <masl/Logger.h>
#include "openGL_functions.h"

namespace mar {

    void Shape::render(const matrix & theMatrix) const {
        for (std::vector<ElementPtr>::const_iterator it = elementList.begin(); 
                                                      it != elementList.end(); ++it) {
            ElementPtr element = *it;
            element->loadData(theMatrix);
            glDrawArrays(GL_TRIANGLES, 0, element->numVertices);
            element->unloadData();
            checkGlError("glDrawElements");
        }
    }

    /////////////////////////////////////////////////////////////Element
    void Element::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);

        glEnableVertexAttribArray(VERTEX_POS_INDEX);
        glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 0, (vertexData.get()));
    }

    void Element::unloadData() const {
        glDisableVertexAttribArray(VERTEX_POS_INDEX);
    }

    /////////////////////////////////////////////////////////////ElementWithNormals
    void ElementWithNormals::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);

        int offset = 0;
        int stride = NORMAL_VERTEX_SIZE * (sizeof(float));
        glEnableVertexAttribArray(VERTEX_POS_INDEX);
        glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get()));
        offset += VERTEX_POS_SIZE;
        glEnableVertexAttribArray(VERTEX_NORMAL_INDEX);
        glVertexAttribPointer(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get() + offset));
    }

    void ElementWithNormals::unloadData() const {
        glDisableVertexAttribArray(VERTEX_POS_INDEX);
        glDisableVertexAttribArray(VERTEX_NORMAL_INDEX);
    }

    /////////////////////////////////////////////////////////////ElementWithTexture
    void ElementWithTexture::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);

        int offset = 0;
        int stride = TEXTURED_VERTEX_SIZE;
        glEnableVertexAttribArray(VERTEX_POS_INDEX);
        glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get()));
        offset += VERTEX_POS_SIZE;
        glEnableVertexAttribArray(VERTEX_TEXCOORD0_INDEX);
        glVertexAttribPointer(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get() + offset));
    }

    void ElementWithTexture::unloadData() const {
        glDisableVertexAttribArray(VERTEX_POS_INDEX);
        glDisableVertexAttribArray(VERTEX_TEXCOORD0_INDEX);
    }

    /////////////////////////////////////////////////////////////ElementWithNormalsAndTexture
    void ElementWithNormalsAndTexture::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);

        int offset = 0;
        int stride = TEXTURED_NORMAL_VERTEX_SIZE * (sizeof(float));
        glEnableVertexAttribArray(VERTEX_POS_INDEX);
        glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get()));
        offset += VERTEX_POS_SIZE;
        glEnableVertexAttribArray(VERTEX_NORMAL_INDEX);
        glVertexAttribPointer(VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get() + offset));
        offset += VERTEX_NORMAL_SIZE;
        glEnableVertexAttribArray(VERTEX_TEXCOORD0_INDEX);
        glVertexAttribPointer(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, GL_FLOAT, GL_FALSE, 
                                stride, (vertexData.get() + offset));
    }
    void ElementWithNormalsAndTexture::unloadData() const {
        glDisableVertexAttribArray(VERTEX_POS_INDEX);
        glDisableVertexAttribArray(VERTEX_TEXCOORD0_INDEX);
        glDisableVertexAttribArray(VERTEX_NORMAL_INDEX);
    }
}

