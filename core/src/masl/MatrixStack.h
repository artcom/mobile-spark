// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>
#include <cmath>
#include "numeric_functions.h"


class MatrixStack {
private:
    std::stack<matrix> stack;

    quaternion getQuaterionFrom(const float roll, const float pitch, const float yaw);

public:
    MatrixStack();

    void clear();

    const matrix getTop() const;
    int getSize() const;

    void loadIdentity();
    void loadPerspective(float l, float r, float b, float t, float n, float f);
    void loadOrtho(float l, float r, float b, float t, float n, float f);

    void loadMatrix(const matrix matrix);

    void multMatrix(const matrix matrix);
    void multMatrixLocal(const matrix matrix);

    void pop();
    void push();

    void rotateAxis(const vector3 vector, const float angle);
    void rotateAxisLocal(const vector3 vector, const float angle);
    void rotateXAxis(const float angle);
    void rotateXAxisLocal(const float angle);
    void rotateYAxis(const float angle);
    void rotateYAxisLocal(const float angle);
    void rotateZAxis(const float angle);
    void rotateZAxisLocal(const float angle);

    void rotateYawPitchRoll(const float yaw, const float pitch, const float roll);
    void rotateYawPitchRollLocal(const float yaw, const float pitch, const float roll);

    void scale(const float x, const float y, const float z);
    void scaleLocal(const float x, const float y, const float z);

    void translate(const float x, const float y, const float z);
    void translateLocal(const float x, const float y, const float z);
};


#endif // MATRIXSTACK_H
