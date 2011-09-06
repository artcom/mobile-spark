#include "Shape.h"

#include <masl/Logger.h>
#include "openGL_functions.h"
#include "ObjImporter.h"

namespace mar {

    //////////////////////////////////////////////////////////////Shape
    Shape::Shape(const bool theTextureFlag) : _myTextureFlag(theTextureFlag), _myTransparency(Transparency::UNKNOWN) {
        _myBoundingBox.min.zero(); _myBoundingBox.min[3] = 1;
        _myBoundingBox.max.zero(); _myBoundingBox.max[3] = 1;
    }
    Shape::~Shape() {
    }

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
    
    void Shape::initGL() {
        for (std::vector<ElementPtr>::const_iterator it = elementList.begin(); 
                                                      it != elementList.end(); ++it) {
            ElementPtr element = *it;
            if (element && element->material ) {
                 element->material->initGL();
            }

        }
    }

    void Shape::setBoundingBox(vector4 theMin, vector4 theMax) {
        _myBoundingBox.min = theMin;
        _myBoundingBox.max = theMax;
    }

    bool Shape::isTransparent() {
        if (_myTransparency != Transparency::UNKNOWN) {
            return (_myTransparency == Transparency::TRANSPARENT);
        } else {
            for (std::vector<ElementPtr>::const_iterator it = elementList.begin(); 
                                                          it != elementList.end(); ++it) {
                ElementPtr element = *it;
                if (element->material->transparency_) {
                    _myTransparency = Transparency::TRANSPARENT;
                    return true;
                }
            }
            _myTransparency = Transparency::OPAQUE;
            return false;
        }
    }

    //////////////////////////////////////////////////////////////RectangleShape
    RectangleShape::RectangleShape(const bool theTextureFlag, const std::string & theTextureSrc) 
        : Shape(theTextureFlag) {
        ElementPtr myElement;
        MaterialPtr myMaterial;
        if (theTextureFlag) {
            myElement = ElementPtr(new ElementWithTexture());
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(theTextureSrc));
        } else {
            myElement = ElementPtr(new Element());
            myMaterial = UnlitColoredMaterialPtr(new UnlitColoredMaterial());
        }
        myElement->material = myMaterial;
        elementList.push_back(myElement);
        myMaterial->createShader();
        setVertexData();
        _myBoundingBox.max[0] = _myBoundingBox.max[1] = 1.0f;
        initGL();
    }

    RectangleShape::~RectangleShape() {
    }

    void RectangleShape::setVertexData() {
        ElementPtr myElement = elementList[0];
        _myDataPerVertex = 3 + (_myTextureFlag ? 2 : 0);
        myElement->numVertices = 6;
        myElement->vertexData_ = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        int myXYCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

        for (size_t i = 0, l = myElement->numVertices; i < l; ++i) {
            (myElement->vertexData_)[i * _myDataPerVertex + 0] = myXYCoords[i * 2 + 0];
            (myElement->vertexData_)[i * _myDataPerVertex + 1] = myXYCoords[i * 2 + 1];
            (myElement->vertexData_)[i * _myDataPerVertex + 2] = 0;
            if (_myTextureFlag) {
                (myElement->vertexData_)[i * _myDataPerVertex + 3] = myXYCoords[i * 2 + 0];
                (myElement->vertexData_)[i * _myDataPerVertex + 4] = myXYCoords[i * 2 + 1];
            }
        }
    }

    void RectangleShape::setDimensions(const float theWidth, const float theHeight) {
        AC_PRINT << "setDimensions " << theWidth << ", " << theHeight << " " << _myBoundingBox.min << ", " << _myBoundingBox.max;
        ElementPtr myElement = elementList[0];
        myElement->vertexData_[_myDataPerVertex] = theWidth;
        myElement->vertexData_[_myDataPerVertex*3] = theWidth;
        myElement->vertexData_[_myDataPerVertex*4] = theWidth;
        myElement->vertexData_[_myDataPerVertex*2 + 1] = theHeight;
        myElement->vertexData_[_myDataPerVertex*4 + 1] = theHeight;
        myElement->vertexData_[_myDataPerVertex*5 + 1] = theHeight;
        _myBoundingBox.max[0] = theWidth;
        _myBoundingBox.max[1] = theHeight;
        AC_PRINT << "setDimensions " << theWidth << ", " << theHeight << " " << _myBoundingBox.min << ", " << _myBoundingBox.max;
    }

    //////////////////////////////////////////////////////////////ObjShape
    ObjShape::ObjShape() : Shape() {
    }

    ObjShape::~ObjShape() {
    }

    //////////////////////////////////////////////////////////////ShapeFactory
    ShapeFactory::~ShapeFactory() {}
        
    ShapePtr ShapeFactory::createRectangle(const bool theTextureFlag, const std::string & theTextureSrc) {
        return ShapePtr(new RectangleShape(theTextureFlag, theTextureSrc));
    }

    ShapePtr ShapeFactory::createObj(const std::string & theFile) {
        ShapePtr myShape = ShapePtr(new ObjShape());
        ObjImporter::get().importObj(theFile, myShape);
        return myShape;
    }
    
}

