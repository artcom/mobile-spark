#include "Shape.h"

#include <masl/Logger.h>
#include "openGL_functions.h"
#include "ObjImporter.h"

namespace mar {

    //////////////////////////////////////////////////////////////Shape
    Shape::Shape(const bool theTextureFlag) : _myTextureFlag(theTextureFlag) {
        _myBoundingBox.min.zero(); _myBoundingBox.min[3] = 1;
        _myBoundingBox.max.zero(); _myBoundingBox.max[3] = 1;
        _myBoundingBox.max[0] = _myBoundingBox.max[1] = 1.0f;
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
        initGL();
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
        _myBoundingBox.max[0] = theWidth;
        _myBoundingBox.max[1] = theHeight;
    }

    //////////////////////////////////////////////////////////////NinePatchShape
    NinePatchShape::NinePatchShape(const std::string & theTextureSrc, 
        const float theLeftEdge, const float theTopEdge,  const float theRightEdge, const float theBottomEdge):
        Shape(true) {
        ElementPtr myElement = ElementPtr(new ElementWithTexture());
        MaterialPtr myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(theTextureSrc));
        myElement->material = myMaterial;
        elementList.push_back(myElement);
        myMaterial->createShader();
        setVertexData();
        initGL();
    }

    NinePatchShape::~NinePatchShape() {
    }
    
    void NinePatchShape::setVertexData() {
        ElementPtr myElement = elementList[0];
        _myDataPerVertex = 5;
        myElement->numVertices = 6;
        myElement->vertexData = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        int myXYCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

        for (size_t i = 0, l = myElement->numVertices; i < l; ++i) {
            (myElement->vertexData)[i * _myDataPerVertex + 0] = myXYCoords[i * 2 + 0];
            (myElement->vertexData)[i * _myDataPerVertex + 1] = myXYCoords[i * 2 + 1];
            (myElement->vertexData)[i * _myDataPerVertex + 2] = 0;
            (myElement->vertexData)[i * _myDataPerVertex + 3] = myXYCoords[i * 2 + 0];
            (myElement->vertexData)[i * _myDataPerVertex + 4] = myXYCoords[i * 2 + 1];
        }
    }

    void NinePatchShape::setDimensions(const float theWidth, const float theHeight) {
        ElementPtr myElement = elementList[0];
        myElement->vertexData[_myDataPerVertex] = theWidth;
        myElement->vertexData[_myDataPerVertex*3] = theWidth;
        myElement->vertexData[_myDataPerVertex*4] = theWidth;
        myElement->vertexData[_myDataPerVertex*2 + 1] = theHeight;
        myElement->vertexData[_myDataPerVertex*4 + 1] = theHeight;
        myElement->vertexData[_myDataPerVertex*5 + 1] = theHeight;
        _myBoundingBox.max[0] = theWidth;
        _myBoundingBox.max[1] = theHeight;
    }



    //////////////////////////////////////////////////////////////ObjShape
    ObjShape::ObjShape() : Shape() {
    }

    ObjShape::~ObjShape() {
    }

    //bounding box is calculated during obj import and set from there
    void ObjShape::setBoundingBox(vector4 theMin, vector4 theMax) {
        _myBoundingBox.min = theMin;
        _myBoundingBox.max = theMax;
    }

    //////////////////////////////////////////////////////////////ShapeFactory
    ShapeFactory::~ShapeFactory() {}
        
    ShapePtr ShapeFactory::createRectangle(const bool theTextureFlag, const std::string & theTextureSrc) {
        return ShapePtr(new RectangleShape(theTextureFlag, theTextureSrc));
    }

    ShapePtr ShapeFactory::createNinePatch(const std::string & theTextureSrc, 
            const float theLeftEdge, const float theTopEdge,  const float theRightEdge, const float theBottomEdge) {
        return ShapePtr(new NinePatchShape(theTextureSrc, theLeftEdge, theTopEdge, theRightEdge, theBottomEdge));
    }

    ShapePtr ShapeFactory::createObj(const std::string & theFile) {
        ObjShapePtr myShape = ObjShapePtr(new ObjShape());
        ObjImporter::get().importObj(theFile, myShape);
        return myShape;
    }
    
}

