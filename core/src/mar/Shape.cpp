#include "Shape.h"

#include <masl/Logger.h>
#include "openGL_functions.h"
#include "ObjImporter.h"

namespace mar {

    //////////////////////////////////////////////////////////////Shape
    Shape::Shape(const bool theTextureFlag, const float theWidth, const float theHeight) : _myTextureFlag(theTextureFlag), width_(std::max(theWidth, 0.f)), height_(std::max(theHeight, 0.f))
    {
        _myBoundingBox.min.zero(); _myBoundingBox.min[3] = 1;
        _myBoundingBox.max.zero(); _myBoundingBox.max[3] = 1;
        _myBoundingBox.max[0] = _myBoundingBox.max[1] = 1.0f;
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

#ifdef iOS
    void Shape::render(const matrix & theMatrix) const {
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
             it != elementList_.end(); ++it) {

            ElementPtr element = *it;
            element->material->loadShader(theMatrix);

            //Using Vertex Array Objects. not supported by android.
            glBindVertexArrayOES(element->vertexArrayObject);
            glDrawElements(GL_TRIANGLES, element->numIndices, GL_UNSIGNED_SHORT, (void*)0);
            glBindVertexArrayOES(0);
            checkGlError("glDrawElements");
        }
    }
#endif

#ifdef ANDROID
    void Shape::render(const matrix & theMatrix) const {
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
             it != elementList_.end(); ++it) {
            ElementPtr element = *it;
            element->loadData(theMatrix);
            glDrawElements(GL_TRIANGLES, element->numIndices, GL_UNSIGNED_SHORT, 0);
            element->unloadData();
            checkGlError("glDrawElements");
        }
    }
#endif

    void Shape::updateHandles(const std::map<std::string, float> & theShaderValues) {
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
                                                      it != elementList_.end(); ++it) {
            (*it)->material->setCustomValues(theShaderValues);
        }
    }

    void Shape::initGL() {
        AC_DEBUG << "Shape::init GL";
        for (std::vector<ElementPtr>::const_iterator it = elementList_.begin();
                                                      it != elementList_.end(); ++it) {

            ElementPtr element = *it;
            if (element && element->material ) {
                 element->material->initGL();
                 glLinkProgram(element->material->shaderProgram);
            }
            element->createVertexBuffers();
        }
    }

    void Shape::setBoundingBox(const vector4 & theMin, const vector4 & theMax) {
        _myBoundingBox.min = theMin;
        _myBoundingBox.max = theMax;
    }

    bool Shape::isTransparent() const {
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
    RectangleShape::RectangleShape(const bool theTextureFlag, const float theWidth, const float theHeight, 
                                   const std::string & theVertexShader, const std::string & theFragmentShader, 
                                   const std::vector<std::string> & theCustomHandles,
                                   const std::string & theTextureSrc)
        : Shape(theTextureFlag, theWidth, theHeight)
    {

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
        elementList_.push_back(myElement);
        myMaterial->createShader(theVertexShader, theFragmentShader);
        setVertexData(vector2(0,0), vector2(theWidth, theHeight));
        initGL();
        myMaterial->setCustomHandles(theCustomHandles);
        _myBoundingBox.max[0] = theWidth;
        _myBoundingBox.max[1] = theHeight;
    }

    RectangleShape::~RectangleShape() {
    }
    
    
    void RectangleShape::setVertexData(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {
        ElementPtr myElement = elementList_[0];
        _myDataPerVertex = 3 + (_myTextureFlag ? 2 : 0);
        myElement->numVertices = 4;
        myElement->numIndices = 6;
        myElement->vertexData_ = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        GLushort indices[] = {0, 1, 2, 2, 1, 3};
        myElement->indexDataVBO_ = boost::shared_array<GLushort>(new GLushort[myElement->numIndices]);
        float myXYCoords[] = {0.0f, 0.0f,1.0f, 0.0f, 0.0f, 1.0f,1.0f, 1.0f};

        for (size_t i = 0, l = myElement->numVertices; i < l; ++i) {
            (myElement->vertexData_)[i * _myDataPerVertex + 0] = myXYCoords[i * 2 + 0] * width_;
            (myElement->vertexData_)[i * _myDataPerVertex + 1] = myXYCoords[i * 2 + 1] * height_;
            (myElement->vertexData_)[i * _myDataPerVertex + 2] = 0;

            if (_myTextureFlag) {
                (myElement->vertexData_)[i * _myDataPerVertex + 3] = myXYCoords[i * 2 + 0];
                (myElement->vertexData_)[i * _myDataPerVertex + 4] = myXYCoords[i * 2 + 1];
            }
        }

        for (size_t i = 0; i < myElement->numIndices; i++) {
            myElement->indexDataVBO_[i] = indices[i];
        }
    }

    void RectangleShape::setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {
        ElementPtr myElement = elementList_[0];

        myElement->vertexData_[0] = theLowerLeftCorner[0];
        myElement->vertexData_[0 + 1] = theLowerLeftCorner[1];
        
        myElement->vertexData_[_myDataPerVertex] = theUpperRightCorner[0];
        myElement->vertexData_[_myDataPerVertex + 1] = theLowerLeftCorner[1];
        
        myElement->vertexData_[_myDataPerVertex*2] = theLowerLeftCorner[0];
        myElement->vertexData_[(_myDataPerVertex*2) + 1] = theUpperRightCorner[1];

        myElement->vertexData_[_myDataPerVertex*3] = theUpperRightCorner[0];
        myElement->vertexData_[(_myDataPerVertex*3) + 1] =  theUpperRightCorner[1];

        width_ = theUpperRightCorner[0] - theLowerLeftCorner[0];
        height_ = theUpperRightCorner[1] - theLowerLeftCorner[1];
        _myBoundingBox.min[0] = theLowerLeftCorner[0];
        _myBoundingBox.min[1] = theLowerLeftCorner[1];
        _myBoundingBox.max[0] = theUpperRightCorner[0];
        _myBoundingBox.max[1] = theUpperRightCorner[1];
        
        myElement->updateCompleteVertexBuffersContent();
    }
    
    void RectangleShape::setTexCoords(const vector2 & theUV0, const vector2 & theUV1, const vector2 & theUV2, const vector2 & theUV3) {
        ElementPtr myElement = elementList_[0];
        if (_myTextureFlag) {
            (myElement->vertexData_)[0 * _myDataPerVertex + 3] = theUV0[0];
            (myElement->vertexData_)[0 * _myDataPerVertex + 4] = theUV0[1];

            (myElement->vertexData_)[1 * _myDataPerVertex + 3] = theUV1[0];
            (myElement->vertexData_)[1 * _myDataPerVertex + 4] = theUV1[1];

            (myElement->vertexData_)[2 * _myDataPerVertex + 3] = theUV2[0];
            (myElement->vertexData_)[2 * _myDataPerVertex + 4] = theUV2[1];

            (myElement->vertexData_)[3 * _myDataPerVertex + 3] = theUV3[0];
            (myElement->vertexData_)[3 * _myDataPerVertex + 4] = theUV3[1];
            myElement->updateCompleteVertexBuffersContent();
        }
    }

    //////////////////////////////////////////////////////////////NinePatchShape
    NinePatchShape::NinePatchShape(const std::string & theTextureSrc,
        const float theLeftEdge, const float theTopEdge,  const float theRightEdge, const float theBottomEdge,
        const float theWidth, const float theHeight):
        Shape(true, theWidth, theHeight),
        leftEdge_(theLeftEdge), topEdge_(theTopEdge), rightEdge_(theRightEdge), bottomEdge_(theBottomEdge) 
    {
        ElementPtr myElement = ElementPtr(new ElementWithTexture());
        UnlitTexturedMaterialPtr myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(theTextureSrc));
        myElement->material = myMaterial;
        myMaterial->createShader();
        elementList_.push_back(myElement);

        imageWidth_ = myMaterial->getTexture()->width_;
        imageWidth_ = imageWidth_ > 0 ? imageWidth_ : 1;
        imageHeight_ = myMaterial->getTexture()->height_;
        imageHeight_ = imageHeight_ > 0 ? imageHeight_ : 1;

        setVertexData(vector2(0,0), vector2(theWidth, theHeight));
        initGL();
        _myBoundingBox.max[0] = theWidth;
        _myBoundingBox.max[1] = theHeight;
    }

    NinePatchShape::~NinePatchShape() {
    }

    void NinePatchShape::setVertexData(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {
        ElementPtr myElement = elementList_[0];
        _myDataPerVertex = 5; //position and texcoord
        size_t vertices_per_side = 4;
        myElement->numIndices = 54; //9 quads * 2 triangles per quad * 3 vertices per triangle
        myElement->numVertices = vertices_per_side * vertices_per_side;
        myElement->vertexData_ = boost::shared_array<float>(new float[(myElement->numVertices) * _myDataPerVertex]);
        myElement->indexDataVBO_ = boost::shared_array<GLushort>(new GLushort[(myElement->numIndices)]);
        for (size_t i = 0, l = vertices_per_side; i < l; ++i) {
            for (size_t j = 0, m = vertices_per_side; j < m; ++j) {
                float myX, myY;
                float myS, myT;
                if (j== 0) {
                    myX = theLowerLeftCorner[0];
                    myS = 0;
                } else if (j == vertices_per_side - 3) {
                    myX = cml::clamp(leftEdge_ , theLowerLeftCorner[0] , theUpperRightCorner[0]);
                    myS = cml::clamp(leftEdge_/imageWidth_, 0.0f, 1.0f);
                } else if (j == vertices_per_side - 2) {
                    myX = cml::clamp(theUpperRightCorner[0] - rightEdge_, std::min(leftEdge_, theUpperRightCorner[0]), theUpperRightCorner[0]);
                    myS = cml::clamp((imageWidth_ - rightEdge_)/imageWidth_, 0.0f, 1.0f);
                } else if (j == vertices_per_side - 1) {
                    myX = cml::clamp(theUpperRightCorner[0], std::min(leftEdge_, theUpperRightCorner[0]), theUpperRightCorner[0]);
                    myS = 1.0f;
                }
                if (i == 0) {
                    myY = theLowerLeftCorner[1];
                    myT = 0.0f;
                } else if (i == vertices_per_side - 3) {
                    myY = cml::clamp(bottomEdge_,theLowerLeftCorner[1],theUpperRightCorner[1]);
                    myT = cml::clamp(bottomEdge_/imageHeight_, 0.0f, 1.0f);
                } else if (i == vertices_per_side - 2) {
                    myY = cml::clamp(theUpperRightCorner[1] - topEdge_, std::min(bottomEdge_,theUpperRightCorner[1]),theUpperRightCorner[1]);
                    myT = cml::clamp((imageHeight_-topEdge_)/imageHeight_, 0.0f, 1.0f);
                } else if (i == vertices_per_side - 1) {
                    myY = cml::clamp(theUpperRightCorner[1], std::min(bottomEdge_,theUpperRightCorner[1]),theUpperRightCorner[1]);
                    myT = 1.0f;
                }

                size_t v = i * vertices_per_side + j;
                myElement->vertexData_[v * _myDataPerVertex + 0] = myX;
                myElement->vertexData_[v * _myDataPerVertex + 1] = myY;
                myElement->vertexData_[v * _myDataPerVertex + 2] = 0;
                myElement->vertexData_[v * _myDataPerVertex + 3] = myS;
                myElement->vertexData_[v * _myDataPerVertex + 4] = myT;
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

    void NinePatchShape::setDimensions(const vector2 & theLowerLeftCorner, const vector2 & theUpperRightCorner) {
        width_ = theUpperRightCorner[0] - theLowerLeftCorner[0];
        height_ = theUpperRightCorner[1] - theLowerLeftCorner[1];

        _myBoundingBox.min[0] = theLowerLeftCorner[0];
        _myBoundingBox.min[1] = theLowerLeftCorner[1];
        _myBoundingBox.max[0] = theUpperRightCorner[0];
        _myBoundingBox.max[1] = theUpperRightCorner[1];

        setVertexData(theLowerLeftCorner, theUpperRightCorner);
        ElementPtr myElement = elementList_[0];        
        myElement->updateCompleteVertexBuffersContent();
        
    }



    //////////////////////////////////////////////////////////////ObjShape
    ObjShape::ObjShape() : Shape() {
    }

    ObjShape::~ObjShape() {
    }

    //////////////////////////////////////////////////////////////ShapeFactory
    ShapeFactory::~ShapeFactory() {}

    ShapePtr ShapeFactory::createRectangle(const bool theTextureFlag, const float theWidth, const float theHeight,
                                           const std::string & theVertexShader, const std::string & theFragmentShader,
                                           const std::vector<std::string> & theCustomHandles,
                                           const std::string & theTextureSrc) {
        return ShapePtr(new RectangleShape(theTextureFlag, theWidth, theHeight, 
                            theVertexShader, theFragmentShader, theCustomHandles, theTextureSrc));
    }

    ShapePtr ShapeFactory::createNinePatch(const std::string & theTextureSrc,
            const float theLeftEdge, const float theTopEdge,  const float theRightEdge, const float theBottomEdge,
            const float theWidth, const float theHeight) {
        return ShapePtr(new NinePatchShape(theTextureSrc, theLeftEdge, theTopEdge, theRightEdge, theBottomEdge,
                                           theWidth, theHeight));
    }

    ShapePtr ShapeFactory::createObj(const std::string & theFile) {
        ShapePtr myShape = ShapePtr(new ObjShape());
        ObjImporter::get().importObj(theFile, myShape);
        myShape->initGL();
        return myShape;
    }

}

