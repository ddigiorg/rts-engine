#ifndef CAMERA_H
#define CAMERA_H

// Local includes
#include "types.h"

// third party includes
#include <GL/glew.h>
#include <SDL.h>

// STL includes
#include <vector>
#include <iostream>

// =============================================================================
// Camera Class
// =============================================================================
class Camera {
    public:
        Camera() {};
        void initOrthographic(int width, int height);
        void initPerspective(int width, int height);
        void initView(float x, float y, float z);
        void moveView(float x, float y);
        void moveView(float x, float y, float z);
        void zoomView(const float zoom);

    public:
        float zoom;
        vec3f_t pos;
        vec2i_t resolution;
        mat4x4f_t projMat;
        mat4x4f_t viewMat;
};

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
    zoom = 1.0f;

    viewMat.m00 = zoom; // scale x
    viewMat.m01 = 0.0f;
    viewMat.m02 = 0.0f;
    viewMat.m03 = 0.0f;
    viewMat.m10 = 0.0f;
    viewMat.m11 = zoom; // scale y
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
// Move View Matrix
// =============================================================================
void Camera::moveView(float x, float y) {

    pos.x = x;
    pos.y = y;

    viewMat.m30 = -x; // TODO: why negative?
    viewMat.m31 = -y;
}

// =============================================================================
// Move View Matrix
// =============================================================================
void Camera::moveView(float x, float y, float z) {

    pos.x = x;
    pos.y = y;
    pos.z = z;

    viewMat.m30 = -x; // TODO: why negative?
    viewMat.m31 = -y;
    viewMat.m32 = -z;
}

// =============================================================================
// Zoom View Matrix
// =============================================================================
void Camera::zoomView(const float zoom) {

    if (zoom < 0.1f) {
        this->zoom = 0.1f;
    }
    else if (zoom > 2.0f) {
        this->zoom = 2.0f;
    }
    else {
        this->zoom = zoom;
    }

    viewMat.m00 = this->zoom;
    viewMat.m11 = this->zoom;
}

#endif // CAMERA_H

// generate c++ code to render a triangle in opengl