#include "Shape.h"

#include <masl/Logger.h>
#include "openGL_functions.h"
#include "ObjImporter.h"

namespace mar {

    //////////////////////////////////////////////////////////////Shape
    Shape::Shape(const bool theTextureFlag) : _myTextureFlag(theTextureFlag) {
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

    //////////////////////////////////////////////////////////////RectangleShape
    RectangleShape::RectangleShape(const bool theTextureFlag) : Shape(theTextureFlag) {
        ElementPtr myElement;
        MaterialPtr myMaterial;
        if (theTextureFlag) {
            myElement = ElementPtr(new ElementWithTexture());
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial());
        } else {
            myElement = ElementPtr(new Element());
            myMaterial = UnlitColoredMaterialPtr(new UnlitColoredMaterial());
        }
        myElement->material = myMaterial;
        elementList.push_back(myElement);
        myMaterial->createShader();
        setVertexData();
    }

    RectangleShape::~RectangleShape() {
    }

    void RectangleShape::setVertexData() {
        ElementPtr myElement = elementList[0];
        _myDataPerVertex = 3 + (_myTextureFlag ? 2 : 0);
        myElement->numVertices = 6;
        myElement->vertexData = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        int myXYCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

        for (size_t i = 0, l = myElement->numVertices; i < l; ++i) {
            (myElement->vertexData)[i * _myDataPerVertex + 0] = myXYCoords[i * 2 + 0];
            (myElement->vertexData)[i * _myDataPerVertex + 1] = myXYCoords[i * 2 + 1];
            (myElement->vertexData)[i * _myDataPerVertex + 2] = 0;
            if (_myTextureFlag) {
                (myElement->vertexData)[i * _myDataPerVertex + 3] = myXYCoords[i * 2 + 0];
                (myElement->vertexData)[i * _myDataPerVertex + 4] = myXYCoords[i * 2 + 1];
            }
        }
    }

    void RectangleShape::setDimensions(const float theWidth, const float theHeight) {
        ElementPtr myElement = elementList[0];
        myElement->vertexData[_myDataPerVertex] = theWidth;
        myElement->vertexData[_myDataPerVertex*3] = theWidth;
        myElement->vertexData[_myDataPerVertex*4] = theWidth;
        myElement->vertexData[_myDataPerVertex*2 + 1] = theHeight;
        myElement->vertexData[_myDataPerVertex*4 + 1] = theHeight;
        myElement->vertexData[_myDataPerVertex*5 + 1] = theHeight;
    }

    //////////////////////////////////////////////////////////////ObjShape
    ObjShape::ObjShape() {
    }

    ObjShape::~ObjShape() {
    }

    //////////////////////////////////////////////////////////////ShapeFactory
    ShapeFactory::~ShapeFactory() {}
        
    ShapePtr ShapeFactory::createRectangle(const bool theTextureFlag) {
        return ShapePtr(new RectangleShape(theTextureFlag));
    }

    ShapePtr ShapeFactory::createObj(const std::string & theFile) {
        ShapePtr myShape = ShapePtr(new ObjShape());
        ObjImporter::get().importObj(theFile, myShape);
        return myShape;
    }
    
}

