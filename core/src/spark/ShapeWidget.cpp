#include "ShapeWidget.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <mar/Shape.h>
#include <mar/Material.h>
#include <animation/AnimationManager.h>

#define DB(x) // x

namespace spark {
    ShapeWidget::ShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Widget(theApp, theXMLNode), _myOrigin(vector2(0,0)), _myOriginMode(NO_ORIGIN) 
    {
        vertexShader_ = getNode()->getAttributeAs<std::string>("vertex_shader","");
        fragmentShader_ = getNode()->getAttributeAs<std::string>("fragment_shader","");
        std::string customShaderHandles = getNode()->getAttributeAs<std::string>("custom_shader_handles","");
        std::istringstream iss(customShaderHandles);
        std::vector<std::string> myHandles;
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string> >(myHandles));
        for (std::vector<std::string>::iterator it = myHandles.begin(); it != myHandles.end(); ++it) {
            customShaderValues_[(*it)] = 0.0f;
        }        
        std::string myOriginStr = getNode()->getAttributeAs<std::string>("origin","");
        if (myOriginStr == "") {
            _myOriginMode = NO_ORIGIN;
        } else if (myOriginStr == "center") {
            _myOriginMode = CENTER_ORIGIN;
        } else {
            _myOriginMode = EXPLICIT_ORIGIN;
            _myOrigin = masl::as<vector2>(myOriginStr);
        }
    }

    ShapeWidget::~ShapeWidget() {}

    mar::ShapePtr
    ShapeWidget::createCustomShape(const mar::MaterialPtr theMaterial) {
        return mar::ShapePtr(new mar::RectangleShape(theMaterial));
    }


    void 
    ShapeWidget::prerender(MatrixStack& theCurrentMatrixStack) {
        Widget::prerender(theCurrentMatrixStack);
        if (isVisible() && customShaderValues_.size() > 0) {
            _myShape->updateCustomHandles(customShaderValues_);
        }
    }

    void
    ShapeWidget::render(const matrix & theP) const {
        matrix mvp = theP * _myWorldMVMatrix;
        _myShape->render(mvp);
        Widget::render(theP);
    }

    void
    ShapeWidget::propagateAlpha() {
        Widget::propagateAlpha(); 
        if (getShape()) {
            AC_DEBUG << *this << " set calculated alpha: " << getActualAlpha();
            getShape()->setAlpha(getActualAlpha());
        }
    }
    
    bool
    ShapeWidget::AABB2Dcontains(const float x, const float y,
                                     const matrix & theProjectionMatrix) const {
        mar::BoundingBox myBB = _myShape->getBoundingBox();
        //use 8 corner points
        vector4 corners[8];
        corners[0][0] = myBB.min[0];
        corners[0][1] = myBB.min[1];
        corners[0][2] = myBB.min[2];
        corners[0][3] = 1;
        corners[1][0] = myBB.min[0];
        corners[1][1] = myBB.min[1];
        corners[1][2] = myBB.max[2];
        corners[1][3] = 1;
        corners[2][0] = myBB.min[0];
        corners[2][1] = myBB.max[1];
        corners[2][2] = myBB.min[2];
        corners[2][3] = 1;
        corners[3][0] = myBB.min[0];
        corners[3][1] = myBB.max[1];
        corners[3][2] = myBB.max[2];
        corners[3][3] = 1;
        corners[4][0] = myBB.max[0];
        corners[4][1] = myBB.min[1];
        corners[4][2] = myBB.min[2];
        corners[4][3] = 1;
        corners[5][0] = myBB.max[0];
        corners[5][1] = myBB.min[1];
        corners[5][2] = myBB.max[2];
        corners[5][3] = 1;
        corners[6][0] = myBB.max[0];
        corners[6][1] = myBB.max[1];
        corners[6][2] = myBB.min[2];
        corners[6][3] = 1;
        corners[7][0] = myBB.max[0];
        corners[7][1] = myBB.max[1];
        corners[7][2] = myBB.max[2];
        corners[7][3] = 1;
        matrix mvp = theProjectionMatrix * _myWorldMVMatrix;
        for (size_t i = 0; i < 8; ++i) {
            corners[i] = mvp * corners[i];
        }
        DB(AC_DEBUG << "AABB2D " << getName();)
        DB(AC_DEBUG << " bounding box " << myBB.min << ", " << myBB.max;)
        DB(AC_DEBUG << "mv " << _myWorldMVMatrix << "p " << theProjectionMatrix << " final " << mvp;)

        mar::BoundingBox myMVPBB;
        myMVPBB.min = corners[0];
        myMVPBB.max = corners[0];
        for (size_t i = 1; i < 8; ++i) {
            if (corners[i][0] < myMVPBB.min[0]) {
                myMVPBB.min[0] = corners[i][0];
            }
            if (corners[i][1] < myMVPBB.min[1]) {
                myMVPBB.min[1] = corners[i][1];
            }
            if (corners[i][0] > myMVPBB.max[0]) {
                myMVPBB.max[0] = corners[i][0];
            }
            if (corners[i][1] > myMVPBB.max[1]) {
                myMVPBB.max[1] = corners[i][1];
            }
        }
        DB(AC_DEBUG << " projected bounding box " << myMVPBB.min << ", " << myMVPBB.max;)

        //divide by w -> TODO: find cml method
        myMVPBB.min[0] /= myMVPBB.min[3];
        myMVPBB.min[1] /= myMVPBB.min[3];
        myMVPBB.min[2] /= myMVPBB.min[3];
        myMVPBB.min[3] /= myMVPBB.min[3];
        myMVPBB.max[0] /= myMVPBB.min[3];
        myMVPBB.max[1] /= myMVPBB.min[3];
        myMVPBB.max[2] /= myMVPBB.min[3];
        myMVPBB.max[3] /= myMVPBB.min[3];

        //+1/2
        myMVPBB.min[0] = (myMVPBB.min[0] + 1) / 2;
        myMVPBB.min[1] = (myMVPBB.min[1] + 1) / 2;
        myMVPBB.max[0] = (myMVPBB.max[0] + 1) / 2;
        myMVPBB.max[1] = (myMVPBB.max[1] + 1) / 2;
        DB(AC_DEBUG << " projected bounding box " << myMVPBB.min << ", " << myMVPBB.max;)
        DB(AC_DEBUG << "pick at " << x << ", " << y;)

        if (x >= myMVPBB.min[0] && x <= myMVPBB.max[0] &&
            y >= myMVPBB.min[1] && y <= myMVPBB.max[1]) {
            return true;
        }
        return false;
    }

    //ANDROID ONLY: gl context is lost, so reset all buffers/shaders/textures to zero to create new ones
    void
    ShapeWidget::onPause() {
        Widget::onPause();
        if (_myShape) {
            _myShape->resetGL();
        }
    }

    void
    ShapeWidget::onResume() {
        Widget::onResume();
        _myDirtyFlag = true;
    }

    void
    ShapeWidget::setSize(const vector2 & theSize) {
        if (getShape()) {
            if (_myOriginMode == NO_ORIGIN) {
                _myOrigin = vector2(0,0);
            } else if (_myOriginMode == CENTER_ORIGIN) {
                _myOrigin = theSize * 0.5;
            }
            vector2 myLowerLeftCorner = _myOrigin * -1.0f;
            vector2 myUpperRightCorner = theSize - _myOrigin;
            getShape()->setDimensions(myLowerLeftCorner, myUpperRightCorner);
        }
    }

    float 
    ShapeWidget::getWorldZ() const {
        vector3 myTranslation = matrix_get_translation(_myWorldMVMatrix);
        return myTranslation[2];
    }

    std::string 
    ShapeWidget::getAttributesAsString() const {
        return Widget::getAttributesAsString() + " origin=\""+masl::as_string(_myOrigin)+"\""
                " originMode=\""+masl::as_string(_myOriginMode)+"\""
                " shape={"+(_myShape?_myShape->getAttributesAsString():"")+"}";
    }

}
