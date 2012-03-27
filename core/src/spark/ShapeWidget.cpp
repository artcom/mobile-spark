// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ShapeWidget.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <mar/Material.h>
#include <animation/AnimationManager.h>

#define DB(x) // x

namespace spark {
    ShapeWidget::ShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode) :
        Widget(theApp, theXMLNode),
        _myForcedSize(getNode()->getAttributeAs<float>("width", -1), getNode()->getAttributeAs<float>("height", -1)),
        _myVertexShader(getNode()->getAttributeAs<std::string>("vertex_shader","")),
        _myFragmentShader(getNode()->getAttributeAs<std::string>("fragment_shader","")),
        _myOrigin(vector2(0,0)),
        _myOriginMode(NO_ORIGIN)
    {
        std::string customShaderHandles = getNode()->getAttributeAs<std::string>("custom_shader_handles","");
        std::istringstream iss(customShaderHandles);
        std::vector<std::string> myHandles;
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string> >(myHandles));
        for (std::vector<std::string>::iterator it = myHandles.begin(); it != myHandles.end(); ++it) {
            _myCustomShaderValues[(*it)] = 0.0f;
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
        if (isVisible() && !_myCustomShaderValues.empty()) {
            _myShape->updateCustomHandles(_myCustomShaderValues);
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
    void 
    ShapeWidget::makeMVPBB(mar::BoundingBox & theBB, const matrix & theProjectionMatrix) const{
                                    
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

        theBB.min = corners[0];
        theBB.max = corners[0];
        for (size_t i = 1; i < 8; ++i) {
            if (corners[i][0] < theBB.min[0]) {
                theBB.min[0] = corners[i][0];
            }
            if (corners[i][1] < theBB.min[1]) {
                theBB.min[1] = corners[i][1];
            }
            if (corners[i][0] > theBB.max[0]) {
                theBB.max[0] = corners[i][0];
            }
            if (corners[i][1] > theBB.max[1]) {
                theBB.max[1] = corners[i][1];
            }
        }
        DB(AC_DEBUG << " projected bounding box " << theBB.min << ", " << theBB.max;)

        //divide by w -> TODO: find cml method
        theBB.min[0] /= theBB.min[3];
        theBB.min[1] /= theBB.min[3];
        theBB.min[2] /= theBB.min[3];
        theBB.min[3] /= theBB.min[3];
        theBB.max[0] /= theBB.min[3];
        theBB.max[1] /= theBB.min[3];
        theBB.max[2] /= theBB.min[3];
        theBB.max[3] /= theBB.min[3];

        //+1/2
        theBB.min[0] = (theBB.min[0] + 1) / 2;
        theBB.min[1] = (theBB.min[1] + 1) / 2;
        theBB.max[0] = (theBB.max[0] + 1) / 2;
        theBB.max[1] = (theBB.max[1] + 1) / 2;
        
    }
    
    /** Returns TRUE if at least one point exists that is contained by both boxes;
    */    
    bool
    ShapeWidget::touches2DScreenCoords( mar::BoundingBox & theBB,
                                const matrix & theProjectionMatrix) const {
        mar::BoundingBox myMVPBB;
        makeMVPBB(myMVPBB, theProjectionMatrix);
        bool myTouch = myMVPBB.touches2D(theBB);
        return myTouch;
    }
    
    bool
    ShapeWidget::AABB2Dcontains(const float x, const float y,
                                const matrix & theProjectionMatrix) const {
        mar::BoundingBox myMVPBB;
        makeMVPBB(myMVPBB, theProjectionMatrix);
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
        if (_myDirtyFlag) {
            build();
        }        
        if (getShape()) {
            if (_myOriginMode == NO_ORIGIN) {
                _myOrigin = vector2(0,0);
            } else if (_myOriginMode == CENTER_ORIGIN) {
                _myOrigin = theSize * 0.5;
            }
            vector2 myLowerLeftCorner = _myOrigin * -1.0f;
            vector2 myUpperRightCorner = theSize - _myOrigin;
            getShape()->setDimensions(myLowerLeftCorner, myUpperRightCorner);
            _myForcedSize = theSize; // XXX: hmm?!
        }
    }
    
    const vector2
    ShapeWidget::getSize() {
        if (_myDirtyFlag) {
            build();
        }        
        return vector2(getShape()->getWidth(), getShape()->getHeight());
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
