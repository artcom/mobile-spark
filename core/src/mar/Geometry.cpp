#include "Geometry.h"

#include <masl/Logger.h>
#include "openGL_functions.h"

namespace mar {

    /////////////////////////////////////////////////////////////Shape
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
    Element::Element() {
        _myConfig.push_back(std::pair<unsigned int, unsigned int>(VERTEX_POS_INDEX, VERTEX_POS_SIZE));
        _myStride = 0;
    }

    Element::~Element() {
    }

    void Element::loadData(const matrix & theMatrix) const {
        material->loadShader(theMatrix);
        int offset = 0;
        for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) { 
            glEnableVertexAttribArray(it->first);
            glVertexAttribPointer(it->first, it->second, GL_FLOAT, GL_FALSE, _myStride, (vertexData.get() + offset));
            offset += it->second;
        }
    }

    void Element::unloadData() const {
        for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = _myConfig.begin(); it != _myConfig.end(); ++it) { 
            glDisableVertexAttribArray(it->first);
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

