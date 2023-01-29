#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"

#include <GL/glew.h>
#include <SDL.h>

#include <vector>

// =============================================================================
// Camera Class
// =============================================================================
class Camera {
    public:
        Camera();
        void initOrthographic(int width, int height);
        void initPerspective(int width, int height);
        void initView(float x, float y, float z);
        void moveView(float x, float y);
        void moveView(float x, float y, float z);

    public:
        vec3f_t pos;
        vec2i_t resolution;
        mat4x4f_t projMat;
        mat4x4f_t viewMat;
};

// =============================================================================
// Construct Camera
// =============================================================================
Camera::Camera() {

}

// =============================================================================
// Initialize Orthographic Projection Matrix
// =============================================================================
void Camera::initOrthographic(int width, int height) {

    resolution.x = width;
    resolution.y = height;

    GLfloat halfX = GLfloat(width / 2);
    GLfloat halfY = GLfloat(height / 2);

    GLfloat l = -halfX;
    GLfloat r =  halfX;
    GLfloat b =  halfY;
    GLfloat t = -halfY;
    GLfloat n = -1.0f;
    GLfloat f =  1.0f;

    projMat.m00 = 2.0f / (r - l);
    projMat.m01 = 0.0f;
    projMat.m02 = 0.0f;
    projMat.m03 = 0.0f;
    projMat.m10 = 0.0f;
    projMat.m11 = 2.0f / (t - b);
    projMat.m12 = 0.0f;
    projMat.m13 = 0.0f;
    projMat.m20 = 0.0f;
    projMat.m21 = 0.0f;
    projMat.m22 = -2.0f / (f - n);
    projMat.m23 = 0.0f;
    projMat.m30 = -(r + l) / (r - l);
    projMat.m31 = -(t + b) / (t - b);
    projMat.m32 = -(f + n) / (f - n);
    projMat.m33 = 1.0f;
}

// =============================================================================
// Initialize Perspective Projection Matrix
// =============================================================================
void Camera::initPerspective(int width, int height) {

    resolution.x = width;
    resolution.y = height;

    GLfloat halfX = GLfloat(width / 2);
    GLfloat halfY = GLfloat(height / 2);

    GLfloat l = -halfX;
    GLfloat r =  halfX;
    GLfloat b =  halfY;
    GLfloat t = -halfY;
    GLfloat n = 1.0f;
    GLfloat f = 10.0f;

    projMat.m00 = 2.0f * n / (r - l);
    projMat.m01 = 0.0f;
    projMat.m02 = 0.0f;
    projMat.m03 = 0.0f;
    projMat.m10 = 0.0f;
    projMat.m11 = 2.0f * n / (t - b);
    projMat.m12 = 0.0f;
    projMat.m13 = 0.0f;
    projMat.m20 = (r + l) / (r - l);
    projMat.m21 = (t + b) / (t - b);
    projMat.m22 = -(f + n) / (f - n);
    projMat.m23 = -1.0f;
    projMat.m30 = 0.0f;
    projMat.m31 = 0.0f;
    projMat.m32 = -(2 * f * n) / (f - n);
    projMat.m33 = 0.0f;
}

// =============================================================================
// Initialize View Matrix
// =============================================================================
void Camera::initView(float x, float y, float z) {

    pos.x = x;
    pos.y = y;
    pos.z = z;

    viewMat.m00 = 1.0f; // scale x
    viewMat.m01 = 0.0f;
    viewMat.m02 = 0.0f;
    viewMat.m03 = 0.0f;
    viewMat.m10 = 0.0f;
    viewMat.m11 = 1.0f; // scale y
    viewMat.m12 = 0.0f;
    viewMat.m13 = 0.0f;
    viewMat.m20 = 0.0f;
    viewMat.m21 = 0.0f;
    viewMat.m22 = 1.0f; // scale z
    viewMat.m23 = 0.0f;
    viewMat.m30 = x; // x
    viewMat.m31 = y; // y
    viewMat.m32 = z; // z
    viewMat.m33 = 1.0f;
}

// =============================================================================
// Update View Matrix
// =============================================================================
void Camera::moveView(float x, float y) {

    pos.x = x;
    pos.y = y;

    viewMat.m30 = -x; // TODO: why negative?
    viewMat.m31 = -y;
}
// =============================================================================
// Update View Matrix
// =============================================================================
void Camera::moveView(float x, float y, float z) {

    pos.x = x;
    pos.y = y;
    pos.z = z;

    viewMat.m30 = -x; // TODO: why negative?
    viewMat.m31 = -y;
    viewMat.m32 = -z;
}

#endif // CAMERA_H
