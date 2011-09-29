#include "ShapeWidget.h"

namespace spark {
    ShapeWidget::ShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Widget(theApp, theXMLNode)
    {}

    ShapeWidget::~ShapeWidget() {}

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
        //AC_PRINT << "AABB2D " << getName();
        //AC_PRINT << " bounding box " << myBB.min << ", " << myBB.max;
        //AC_PRINT << "mv " << _myWorldMVMatrix << "p " << theProjectionMatrix << " final " << mvp;

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
        //AC_PRINT << " projected bounding box " << myMVPBB.min << ", " << myMVPBB.max;

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
        //AC_PRINT << " projected bounding box " << myMVPBB.min << ", " << myMVPBB.max;
        //AC_PRINT << "pick at " << x << ", " << y;

        if (x >= myMVPBB.min[0] && x <= myMVPBB.max[0] &&
            y >= myMVPBB.min[1] && y <= myMVPBB.max[1]) {
            return true;
        }
        return false;
    }

    void
    ShapeWidget::onResume() {
        Widget::onResume();
        if (_myShape) {
            _myShape->initGL();
        }
    }

    void 
    ShapeWidget::setShape( mar::ShapePtr theShapePtr) {
        _myShape = theShapePtr;
    }

    float 
    ShapeWidget::getWorldZ() const {
        vector3 myTranslation = matrix_get_translation(_myWorldMVMatrix);
        return myTranslation[2];
    }
}
