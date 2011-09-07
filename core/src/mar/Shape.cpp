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
    RectangleShape::RectangleShape(const bool theTextureFlag, const float theWidth, const float theHeight,
                                   const std::string & theTextureSrc) 
        : Shape(theTextureFlag), width_(theWidth), height_(theHeight) {
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
            (myElement->vertexData)[i * _myDataPerVertex + 0] = myXYCoords[i * 2 + 0] * width_;
            (myElement->vertexData)[i * _myDataPerVertex + 1] = myXYCoords[i * 2 + 1] * height_;
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
        width_ = theWidth;
        height_ = theHeight;
    }

    //////////////////////////////////////////////////////////////NinePatchShape
    NinePatchShape::NinePatchShape(const std::string & theTextureSrc, 
        const float theLeftEdge, const float theTopEdge,  const float theRightEdge, const float theBottomEdge, 
        const float theWidth, const float theHeight):
        Shape(true), width_(theWidth), height_(theHeight), leftEdge_(0), topEdge_(0), rightEdge_(0), bottomEdge_(0) {
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
        size_t vertices_per_side = 4;
        myElement->numVertices = 54; //9 quads * 2 triangles per quad * 3 vertices per triangle
        myElement->vertexData = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        for (size_t i = 0, l = vertices_per_side; i < l; ++i) {
            for (size_t j = 0, m = vertices_per_side; j < m; ++j) {
                size_t v = i * vertices_per_side + j;
                float myX, myY; //TODO: origin stuff?
                if (j== 0) {
                    myX = 0;
                } else if (j == vertices_per_side - 3) {
                    myX = masl::clamp(leftEdge_ ,0.0f , width_);
                } else if (j == vertices_per_side - 2) {
                    myX = masl::clamp(width_ - rightEdge_, std::min(leftEdge_, width_), width_);
                } else if (j == vertices_per_side - 1) {
                    myX = masl::clamp(width_, std::min(leftEdge_, width_), width_);
                }
                if (i == 0) {
                    myY = 0;
                } else if (i == vertices_per_side - 3) {
                    myY = masl::clamp(bottomEdge_,0.0f,height_);
                } else if (i == vertices_per_side - 2) {
                    myY = masl::clamp(height_ - topEdge_, std::min(bottomEdge_,height_),height_);
                } else if (i == vertices_per_side - 1) {
                    myY = masl::clamp(height_, std::min(bottomEdge_,height_),height_);
                }

                (myElement->vertexData)[v + 0] = myX;
                (myElement->vertexData)[v + 1] = myY;
                (myElement->vertexData)[v + 2] = 0;
                (myElement->vertexData)[v + 3] = width_ > 0 ? myX/width_ : 0;
                (myElement->vertexData)[v + 4] = height_ > 0 ? myY/height_ : 0;
            }
        }
    }

    void NinePatchShape::setDimensions(const float theWidth, const float theHeight) {
        width_ = theWidth;
        height_ = theHeight;
        _myBoundingBox.max[0] = theWidth;
        _myBoundingBox.max[1] = theHeight;
        setVertexData();
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
        
    ShapePtr ShapeFactory::createRectangle(const bool theTextureFlag, const float theWidth, const float theHeight,
                                           const std::string & theTextureSrc) {
        return ShapePtr(new RectangleShape(theTextureFlag, theWidth, theHeight, theTextureSrc));
    }

    ShapePtr ShapeFactory::createNinePatch(const std::string & theTextureSrc, 
            const float theLeftEdge, const float theTopEdge,  const float theRightEdge, const float theBottomEdge,
            const float theWidth, const float theHeight) {
        return ShapePtr(new NinePatchShape(theTextureSrc, theLeftEdge, theTopEdge, theRightEdge, theBottomEdge,
                                           theWidth, theHeight));
    }

    ShapePtr ShapeFactory::createObj(const std::string & theFile) {
        ObjShapePtr myShape = ObjShapePtr(new ObjShape());
        ObjImporter::get().importObj(theFile, myShape);
        return myShape;
    }
    
}

