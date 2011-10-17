#include "Shape.h"

#include <masl/Logger.h>
#include "openGL_functions.h"
#include "ObjImporter.h"

namespace mar {

    //////////////////////////////////////////////////////////////Shape
    Shape::Shape(const float theWidth, const float theHeight) :
        lowerLeft_(0,0), upperRight_(std::max(theWidth, 0.f), std::max(theHeight, 0.f))
    {
        boundingBox_.min.zero(); boundingBox_.min[3] = 1;
        boundingBox_.max.zero(); boundingBox_.max[3] = 1;
        boundingBox_.max[0] = boundingBox_.max[1] = 1.0f;
    }

    Shape::~Shape() {
    }

    void
    Shape::setAlpha(const float theAlpha) {
        AC_DEBUG << "Shape setAlpha: " << theAlpha;
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
             it != elementList_.end(); ++it)
        {
            (*it)->material->setAlpha(theAlpha);
        }
    }

    void
    Shape::render(const matrix & theMatrix) const {
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
             it != elementList_.end(); ++it) {
            ElementPtr element = *it;
            element->loadData(theMatrix);
            element->draw();
            element->unloadData();
        }
    }

    void
    Shape::updateHandles(const std::map<std::string, float> & theShaderValues) {
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
                                                      it != elementList_.end(); ++it) {
            (*it)->material->setCustomValues(theShaderValues);
        }
    }

    void
    Shape::initGL() {
        AC_DEBUG << "Shape::init GL";
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
                                                      it != elementList_.end(); ++it) {
            (*it)->initGL();
        }
    }

    void
    Shape::setBoundingBox(const vector4 & theMin, const vector4 & theMax) {
        boundingBox_.min = theMin;
        boundingBox_.max = theMax;
    }

    bool
    Shape::isTransparent() const {
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
                                                      it != elementList_.end(); ++it) {
            ElementPtr element = *it;
            if (element->material->transparency_) {
                return true;
            }
        }
        return false;        
    }

    /////////////////////////////////////////////////////////////RectangleShape
    RectangleShape::RectangleShape(const MaterialPtr theMaterial, const float theWidth, const float theHeight)
        : Shape(theWidth, theHeight)
    {

        ElementPtr myElement;
        if (boost::dynamic_pointer_cast<UnlitTexturedMaterial>(theMaterial)) {
            myElement = ElementPtr(new ElementWithTexture());
        } else {
            myElement = ElementPtr(new Element());
        }
        myElement->material = theMaterial;
        elementList_.push_back(myElement);
        boundingBox_.max[0] = theWidth;
        boundingBox_.max[1] = theHeight;
        setVertexData();
    }

    RectangleShape::~RectangleShape() {
    }
    
    
    void
    RectangleShape::setVertexData() {
        bool myTexturedFlag = (boost::dynamic_pointer_cast<ElementWithTexture>(elementList_[0])) ? true : false;
        AC_PRINT << "textured: "<<myTexturedFlag << " width: "<<getWidth() << " height: "<<getHeight(); 
        ElementPtr myElement = elementList_[0];
        dataPerVertex_ = 3 + (myTexturedFlag ? 2 : 0);
        myElement->numVertices = 4;
        myElement->numIndices = 6;
        myElement->vertexData_ = boost::shared_array<float>(new float[(myElement->numVertices) * dataPerVertex_]);
        GLushort indices[] = {0, 1, 2, 2, 1, 3};
        myElement->indexDataVBO_ = boost::shared_array<GLushort>(new GLushort[myElement->numIndices]);
        float myXYCoords[] = {0.0f, 0.0f,1.0f, 0.0f, 0.0f, 1.0f,1.0f, 1.0f};

        for (size_t i = 0, l = myElement->numVertices; i < l; ++i) {
            (myElement->vertexData_)[i * dataPerVertex_ + 0] = myXYCoords[i * 2 + 0] * getWidth();
            (myElement->vertexData_)[i * dataPerVertex_ + 1] = myXYCoords[i * 2 + 1] * getHeight();
            (myElement->vertexData_)[i * dataPerVertex_ + 2] = 0;

            if (myTexturedFlag) {
                (myElement->vertexData_)[i * dataPerVertex_ + 3] = myXYCoords[i * 2 + 0];
                (myElement->vertexData_)[i * dataPerVertex_ + 4] = myXYCoords[i * 2 + 1];
            }
        }

        for (size_t i = 0; i < myElement->numIndices; i++) {
            myElement->indexDataVBO_[i] = indices[i];
        }
    }

    void
    RectangleShape::setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {
        ElementPtr myElement = elementList_[0];

        myElement->vertexData_[0] = theLowerLeftCorner[0];
        myElement->vertexData_[0 + 1] = theLowerLeftCorner[1];
        
        myElement->vertexData_[dataPerVertex_] = theUpperRightCorner[0];
        myElement->vertexData_[dataPerVertex_ + 1] = theLowerLeftCorner[1];
        
        myElement->vertexData_[dataPerVertex_*2] = theLowerLeftCorner[0];
        myElement->vertexData_[(dataPerVertex_*2) + 1] = theUpperRightCorner[1];

        myElement->vertexData_[dataPerVertex_*3] = theUpperRightCorner[0];
        myElement->vertexData_[(dataPerVertex_*3) + 1] =  theUpperRightCorner[1];

        upperRight_ = theUpperRightCorner;
        lowerLeft_ = theLowerLeftCorner;
        boundingBox_.min[0] = theLowerLeftCorner[0];
        boundingBox_.min[1] = theLowerLeftCorner[1];
        boundingBox_.max[0] = theUpperRightCorner[0];
        boundingBox_.max[1] = theUpperRightCorner[1];
        
        myElement->updateCompleteVertexBuffersContent();
    }
    
    void
    RectangleShape::setTexCoords(const vector2 & theUV0, const vector2 & theUV1, const vector2 & theUV2, const vector2 & theUV3) {
        ElementPtr myElement = elementList_[0];
        if (boost::dynamic_pointer_cast<ElementWithTexture>(myElement)) {
            (myElement->vertexData_)[0 * dataPerVertex_ + 3] = theUV0[0];
            (myElement->vertexData_)[0 * dataPerVertex_ + 4] = theUV0[1];

            (myElement->vertexData_)[1 * dataPerVertex_ + 3] = theUV1[0];
            (myElement->vertexData_)[1 * dataPerVertex_ + 4] = theUV1[1];

            (myElement->vertexData_)[2 * dataPerVertex_ + 3] = theUV2[0];
            (myElement->vertexData_)[2 * dataPerVertex_ + 4] = theUV2[1];

            (myElement->vertexData_)[3 * dataPerVertex_ + 3] = theUV3[0];
            (myElement->vertexData_)[3 * dataPerVertex_ + 4] = theUV3[1];
            myElement->updateCompleteVertexBuffersContent();
        }
    }

    //////////////////////////////////////////////////////////////NinePatchShape
    NinePatchShape::NinePatchShape(const MaterialPtr theMaterial, const float theWidth, const float theHeight):
        Shape(theWidth, theHeight),
        edgeLeft_(0), edgeTop_(0), edgeRight_(0), edgeBottom_(0) 
    {
        ElementPtr myElement = ElementPtr(new ElementWithTexture());
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(theMaterial);
        myElement->material = myMaterial;
        elementList_.push_back(myElement);
        boundingBox_.max[0] = theWidth;
        boundingBox_.max[1] = theHeight;
        imageWidth_ = myMaterial->getTexture()->width_;
        imageWidth_ = imageWidth_ > 0 ? imageWidth_ : 1;
        imageHeight_ = myMaterial->getTexture()->height_;
        imageHeight_ = imageHeight_ > 0 ? imageHeight_ : 1;
        setVertexData();
    }

    NinePatchShape::~NinePatchShape() {
    }

    void
    NinePatchShape::setEdges(const float theLeftEdge, const float theTopEdge,
                  const float theRightEdge, const float theBottomEdge) {
        edgeLeft_ = theLeftEdge;
        edgeTop_ = theTopEdge;
        edgeRight_ = theRightEdge;
        edgeBottom_ = theBottomEdge;
        setVertexData();
    }

    void
    NinePatchShape::setVertexData() {
        ElementPtr myElement = elementList_[0];
        dataPerVertex_ = 5; //position and texcoord
        size_t vertices_per_side = 4;
        myElement->numIndices = 54; //9 quads * 2 triangles per quad * 3 vertices per triangle
        myElement->numVertices = vertices_per_side * vertices_per_side;
        myElement->vertexData_ = boost::shared_array<float>(new float[(myElement->numVertices) * dataPerVertex_]);
        myElement->indexDataVBO_ = boost::shared_array<GLushort>(new GLushort[(myElement->numIndices)]);
        for (size_t i = 0, l = vertices_per_side; i < l; ++i) {
            for (size_t j = 0, m = vertices_per_side; j < m; ++j) {
                float myX, myY;
                float myS, myT;
                if (j== 0) {
                    myX = lowerLeft_[0];
                    myS = 0;
                } else if (j == vertices_per_side - 3) {
                    myX = cml::clamp(edgeLeft_ , lowerLeft_[0] , upperRight_[0]);
                    myS = cml::clamp(edgeLeft_/imageWidth_, 0.0f, 1.0f);
                } else if (j == vertices_per_side - 2) {
                    myX = cml::clamp(upperRight_[0] - edgeRight_, std::min(edgeLeft_, upperRight_[0]), upperRight_[0]);
                    myS = cml::clamp((imageWidth_ - edgeRight_)/imageWidth_, 0.0f, 1.0f);
                } else if (j == vertices_per_side - 1) {
                    myX = cml::clamp(upperRight_[0], std::min(edgeLeft_, upperRight_[0]), upperRight_[0]);
                    myS = 1.0f;
                }
                if (i == 0) {
                    myY = lowerLeft_[1];
                    myT = 0.0f;
                } else if (i == vertices_per_side - 3) {
                    myY = cml::clamp(edgeBottom_,lowerLeft_[1],upperRight_[1]);
                    myT = cml::clamp(edgeBottom_/imageHeight_, 0.0f, 1.0f);
                } else if (i == vertices_per_side - 2) {
                    myY = cml::clamp(upperRight_[1] - edgeTop_, std::min(edgeBottom_,upperRight_[1]),upperRight_[1]);
                    myT = cml::clamp((imageHeight_-edgeTop_)/imageHeight_, 0.0f, 1.0f);
                } else if (i == vertices_per_side - 1) {
                    myY = cml::clamp(upperRight_[1], std::min(edgeBottom_,upperRight_[1]),upperRight_[1]);
                    myT = 1.0f;
                }

                size_t v = i * vertices_per_side + j;
                myElement->vertexData_[v * dataPerVertex_ + 0] = myX;
                myElement->vertexData_[v * dataPerVertex_ + 1] = myY;
                myElement->vertexData_[v * dataPerVertex_ + 2] = 0;
                myElement->vertexData_[v * dataPerVertex_ + 3] = myS;
                myElement->vertexData_[v * dataPerVertex_ + 4] = myT;
            }
        }
        int indices[] = { 0, 1, 4, 4, 1, 5,
                          1, 2, 5, 5, 2, 6,
                          2, 3, 6, 6, 3, 7,
                          4, 5, 8, 8, 5, 9,
                          5, 6, 9, 9, 6,10,
                          6, 7,10,10, 7,11,
                          8, 9,12,12, 9,13,
                          9,10,13,13,10,14,
                         10,11,14,14,11,15 };
        for (size_t i = 0; i < myElement->numIndices; ++i) {
            (myElement->indexDataVBO_)[i] = indices[i];
        }
    }

    void
    NinePatchShape::setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {
        upperRight_ = theUpperRightCorner;
        lowerLeft_ = theLowerLeftCorner;

        boundingBox_.min[0] = theLowerLeftCorner[0];
        boundingBox_.min[1] = theLowerLeftCorner[1];
        boundingBox_.max[0] = theUpperRightCorner[0];
        boundingBox_.max[1] = theUpperRightCorner[1];

        setVertexData();
        ElementPtr myElement = elementList_[0];        
        myElement->updateCompleteVertexBuffersContent();
        
    }



    //////////////////////////////////////////////////////////////ObjShape
    ObjShape::ObjShape() : Shape() {
    }

    ObjShape::~ObjShape() {
    }
}

