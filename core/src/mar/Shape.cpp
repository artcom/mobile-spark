#include "Shape.h"

#include <masl/Logger.h>
#include "openGL_functions.h"
#include "ObjImporter.h"

namespace mar {

    //////////////////////////////////////////////////////////////Shape
    Shape::Shape(const bool theTextureFlag) : _myTextureFlag(theTextureFlag) {
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
            
            /*
            element->material->loadShader(theMatrix);
            glBindBuffer(GL_ARRAY_BUFFER, element->vertexBuffer);
            glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, 0);        
            glEnableVertexAttribArray(ATTRIB_VERTEX);  

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->indexBuffer);
            checkGlError("glDrawElements 0");

            glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
            checkGlError("glDrawElements 1");

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            */
            
            glDrawElements(GL_TRIANGLES, element->numVertices, GL_UNSIGNED_SHORT, 0);
            element->unloadData();
            
            
            checkGlError("glDrawElements 2");
        }
    }
    
    void Shape::initGL() {
        for (std::vector<ElementPtr>::const_iterator it = elementList.begin(); 
                                                      it != elementList.end(); ++it) {
            
            ElementPtr element = *it;
            if (element && element->material ) {
                 element->material->initGL();
            }
            //glBindAttribLocation(element->material->shaderProgram, ATTRIB_VERTEX, "a_position");
            glBindAttribLocation(element->material->shaderProgram, VERTEX_POS_INDEX, "a_position");

            
            AC_PRINT << "ATTRIB_VERTEX: " << ATTRIB_VERTEX;
      
            
            element->createVertexBuffers();
            /*
            glGenBuffers(1, &element->vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, element->vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 12), element->testVertices.get(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            
            glGenBuffers(1, &element->indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(GLushort) * 4), element->testIndices.get(), GL_STATIC_DRAW); 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
             */

        }
    }

    void Shape::setBoundingBox(vector4 theMin, vector4 theMax) {
        _myBoundingBox.min = theMin;
        _myBoundingBox.max = theMax;
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
        myElement->vertexData = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        myElement->indexDataVBO = boost::shared_array<GLushort>(new GLushort[myElement->numVertices]);
        int myXYCoords[] = {0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 50.0f, 50.0f, 0.0f, 50.0f, 50.0f, 0.0f, 50.0f};

        for (size_t i = 0, l = myElement->numVertices; i < l; ++i) {
            (myElement->vertexData)[i * _myDataPerVertex + 0] = myXYCoords[i * 2 + 0];
            (myElement->vertexData)[i * _myDataPerVertex + 1] = myXYCoords[i * 2 + 1];
            (myElement->vertexData)[i * _myDataPerVertex + 2] = 0;
            if (_myTextureFlag) {
                (myElement->vertexData)[i * _myDataPerVertex + 3] = myXYCoords[i * 2 + 0];
                (myElement->vertexData)[i * _myDataPerVertex + 4] = myXYCoords[i * 2 + 1];
            }
            myElement->indexDataVBO[i] = i;
            
        }
    }

    void RectangleShape::setDimensions(const float theWidth, const float theHeight) {
        AC_PRINT << "setDimensions " << theWidth << ", " << theHeight << " " << _myBoundingBox.min << ", " << _myBoundingBox.max;
        ElementPtr myElement = elementList[0];
        myElement->vertexData[_myDataPerVertex] = theWidth;
        myElement->vertexData[_myDataPerVertex*3] = theWidth;
        myElement->vertexData[_myDataPerVertex*4] = theWidth;
        myElement->vertexData[_myDataPerVertex*2 + 1] = theHeight;
        myElement->vertexData[_myDataPerVertex*4 + 1] = theHeight;
        myElement->vertexData[_myDataPerVertex*5 + 1] = theHeight;
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

