#include "MatrixStack.h"

#include <cml/mathlib/matrix_projection.h>
 
// Constructs the matrix and loads the identity matrix.
MatrixStack::MatrixStack() {
    clear();
}

int MatrixStack::getSize() const {
    return stack.size();
}
 
// Clears the stack and pops all matrices.
void MatrixStack::clear() {
    stack.empty();
    matrix startMatrix;
    stack.push(startMatrix);
    loadIdentity();
}
 
// Retrieves the current matrix at the top of the stack.
const matrix MatrixStack::getTop() const {
    return stack.top();
}
 
// Loads identity in the current matrix.
void MatrixStack::loadIdentity() {   
    stack.top() = stack.top().identity();
}
 
void MatrixStack::loadPerspective(float l, float r, float b, float t, float n, float f) {
    matrix perspectiveMatrix;
    cml::matrix_perspective(perspectiveMatrix, l, r, b, t, n, f, cml::right_handed, cml::z_clip_zero);//z_clip_neg_one);
    stack.top() = perspectiveMatrix;
}

void MatrixStack::loadOrtho(float l, float r, float b, float t, float n, float f) {
    matrix orthoMatrix;
    cml::matrix_orthographic(orthoMatrix, l, r, b, t, n, f, cml::right_handed, cml::z_clip_zero);//z_clip_neg_one);
    stack.top() = orthoMatrix;
}


// Loads the specified matrix in the current matrix.
void MatrixStack::loadMatrix(const matrix matrix) {
    stack.top() = matrix;
}
 
// Multiplies the current matrix with the specified matrix.
void MatrixStack::multMatrix(const matrix matrix) {
    stack.top() = stack.top() * matrix;
}
 
// Multiplies the specified matrix with the current matrix.
void MatrixStack::multMatrixLocal(const matrix matrix) {
    stack.top() = matrix * stack.top();
}
 
// Removes the current matrix from the top of the stack.
void MatrixStack::pop() {   
    if (stack.size() > 1) {
        stack.pop();
    }
}
 
// Adds a matrix level to the stack, by duplicating the current matrix.
void MatrixStack::push() {
    stack.push(stack.top());
}
 
// Rotates around an arbitrary vector axis. (world space)
void MatrixStack::rotateAxis(const vector vector, const float angle) {
    matrix rotMatrix;
    matrix_rotation_axis_angle(rotMatrix, vector, angle);
    stack.top() = stack.top() * rotMatrix;
}
 
// Rotates around an arbitrary vector axis. (local space)
void MatrixStack::rotateAxisLocal(const vector vector, const float angle) {
    matrix rotMatrix;
    matrix_rotation_axis_angle(rotMatrix, vector, angle);
    stack.top() = rotMatrix * stack.top();
}

void MatrixStack::rotateXAxis(const float angle) {
    vector v(1,0,0);
    rotateAxis(v,angle);
}
void MatrixStack::rotateYAxis(const float angle) {
    vector v(0,1,0);
    rotateAxis(v,angle);
}
void MatrixStack::rotateZAxis(const float angle) {
    vector v(0,0,1);
    rotateAxisLocal(v,angle);
}
void MatrixStack::rotateXAxisLocal(const float angle) {
    vector v(1,0,0);
    rotateAxisLocal(v,angle);
}
void MatrixStack::rotateYAxisLocal(const float angle) {
    vector v(0,1,0);
    rotateAxisLocal(v,angle);
}
void MatrixStack::rotateZAxisLocal(const float angle) {
    vector v(0,0,1);
    rotateAxisLocal(v,angle);
}

quaternion MatrixStack::getQuaterionFrom(const float roll, const float pitch, const float yaw) {
    float sinRoll = sin(roll/2);
    float cosRoll = cos(roll/2);
    float sinPitch = sin(pitch/2);
    float cosPitch = cos(pitch/2);
    float sinYaw = sin(yaw/2);
    float cosYaw = cos(yaw/2);
    float q0 = cosRoll * cosPitch * cosYaw  +  sinRoll * sinPitch * sinYaw;
    float q1 = sinRoll * cosPitch * cosYaw  -  cosRoll * sinPitch * sinYaw;
    float q2 = cosRoll * sinPitch * cosYaw  +  sinRoll * cosPitch * sinYaw;
    float q3 = cosRoll * cosPitch * sinYaw  -  sinRoll * sinPitch * cosYaw;
    quaternion quaternion(q0, q1, q2, q3);
    return quaternion;
}

// Rotates around an arbitrary axis. (world space)
void MatrixStack::rotateYawPitchRoll(const float yaw, const float pitch, const float roll) {
    quaternion quaternion = getQuaterionFrom(yaw, pitch, roll);
    matrix rotMatrix;
    matrix_rotation_quaternion(rotMatrix, quaternion);
    stack.top() = stack.top() * rotMatrix;
}
 
// Rotates around an arbitrary axis. (local space)
void MatrixStack::rotateYawPitchRollLocal(const float yaw, const float pitch, const float roll) {
    quaternion quaternion = getQuaterionFrom(yaw, pitch, roll);
    matrix rotMatrix;
    matrix_rotation_quaternion(rotMatrix, quaternion);
    stack.top() = rotMatrix * stack.top();
}
 
// Scale the current matrix. (world origin)
void MatrixStack::scale(const float x, const float y, const float z) {
    matrix scaleMatrix;
    matrix_scale(scaleMatrix, x, y, z);
    stack.top() = stack.top() * scaleMatrix;     
}
 
// Scale the current matrix. (local origin)
void MatrixStack::scaleLocal(const float x, const float y, const float z) {
    matrix scaleMatrix;
    matrix_scale(scaleMatrix, x, y, z);
    stack.top() = scaleMatrix * stack.top();     
}
 
// Translate the current matrix. (world origin)
void MatrixStack::translate(const float x, const float y, const float z) {
    matrix transMatrix;
    matrix_translation(transMatrix, x, y, z);
    stack.top() = stack.top() * transMatrix;
}
 
// Translate the current matrix. (local origin)
void MatrixStack::translateLocal(const float x, const float y, const float z) {
    matrix transMatrix;
    matrix_translation(transMatrix, x, y, z);
    stack.top() = transMatrix * stack.top();
}
