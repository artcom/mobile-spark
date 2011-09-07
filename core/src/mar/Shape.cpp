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
        Shape(true), width_(theWidth), height_(theHeight), 
        leftEdge_(theLeftEdge), topEdge_(theTopEdge), rightEdge_(theRightEdge), bottomEdge_(theBottomEdge) {
        ElementPtr myElement = ElementPtr(new ElementWithTexture());
        UnlitTexturedMaterialPtr myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(theTextureSrc));
        myElement->material = myMaterial;
        elementList.push_back(myElement);
        myMaterial->createShader();

        imageWidth_ = myMaterial->getTexture()->width;
        imageWidth_ = imageWidth_ > 0 ? imageWidth_ : 1;
        imageHeight_ = myMaterial->getTexture()->height;
        imageHeight_ = imageHeight_ > 0 ? imageHeight_ : 1;
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
        float gridData[vertices_per_side * vertices_per_side][_myDataPerVertex]; //helpstructure
        for (size_t i = 0, l = vertices_per_side; i < l; ++i) {
            for (size_t j = 0, m = vertices_per_side; j < m; ++j) {
                float myX, myY; //TODO: origin stuff?
                float myS, myT;
                if (j== 0) {
                    myX = 0;
                    myS = 0;
                } else if (j == vertices_per_side - 3) {
                    myX = masl::clamp(leftEdge_ ,0.0f , width_);
                    myS = masl::clamp(leftEdge_/imageWidth_, 0.0f, 1.0f);
                } else if (j == vertices_per_side - 2) {
                    myX = masl::clamp(width_ - rightEdge_, std::min(leftEdge_, width_), width_);
                    myS = masl::clamp((imageWidth_ - rightEdge_)/imageWidth_, 0.0f, 1.0f);
                } else if (j == vertices_per_side - 1) {
                    myX = masl::clamp(width_, std::min(leftEdge_, width_), width_);
                    myS = 1.0f;
                }
                if (i == 0) {
                    myY = 0;
                    myT = 0.0f;
                } else if (i == vertices_per_side - 3) {
                    myY = masl::clamp(bottomEdge_,0.0f,height_);
                    myT = masl::clamp(bottomEdge_/imageHeight_, 0.0f, 1.0f);
                } else if (i == vertices_per_side - 2) {
                    myY = masl::clamp(height_ - topEdge_, std::min(bottomEdge_,height_),height_);
                    myT = masl::clamp((imageHeight_-topEdge_)/imageHeight_, 0.0f, 1.0f);
                } else if (i == vertices_per_side - 1) {
                    myY = masl::clamp(height_, std::min(bottomEdge_,height_),height_);
                    myT = 1.0f;
                }

                gridData[i * vertices_per_side + j][0] = myX;
                gridData[i * vertices_per_side + j][1] = myY;
                gridData[i * vertices_per_side + j][2] = 0;
                gridData[i * vertices_per_side + j][3] = myS;
                gridData[i * vertices_per_side + j][4] = myT;
                //AC_PRINT << i << " " << j << " " << myX << " " << myY << " tex " << myS << " " << myT;
            }
        }
        myElement->vertexData = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        int indices[] = { 0, 1, 4, 4, 1, 5,
                          1, 2, 5, 5, 2, 6,
                          2, 3, 6, 6, 3, 7,
                          4, 5, 8, 8, 5, 9,
                          5, 6, 9, 9, 6,10,
                          6, 7,10,10, 7,11,
                          8, 9,12,12, 9,13,
                          9,10,13,13,10,14,
                         10,11,14,14,11,15 }; 
        for (size_t i = 0; i < myElement->numVertices; ++i) {
            for (size_t j = 0; j < _myDataPerVertex; ++j) {
                (myElement->vertexData)[i * _myDataPerVertex + j] = gridData[indices[i]][j];
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

