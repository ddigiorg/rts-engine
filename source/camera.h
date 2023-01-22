#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <SDL.h>

#include <vector>

#define INIT_POS_X 0.0f
#define INIT_POS_Y 0.0f
#define INIT_POS_Z 0.0f

// =============================================================================
// Camera Class
// =============================================================================
class Camera {
    public:
        Camera(int screenX, int screenY);
        void initOrthographic();
        void initPerspective();
        void initView();
        void moveView(float x, float y);
        void moveView(float x, float y, float z);

    public:
        int screenX;
        int screenY;
        float x;
        float y;
        float z;
        std::vector<GLfloat> projVec;
        std::vector<GLfloat> viewVec;
};

// =============================================================================
// Construct Camera
// =============================================================================
Camera::Camera(int screenX, int screenY) {
    this->screenX = screenX;
    this->screenY = screenY;
    x = INIT_POS_X;
    y = INIT_POS_Y;
    z = INIT_POS_Z;

    projVec.resize(16);
    viewVec.resize(16);
}

// =============================================================================
// Initialize Orthographic Projection Matrix
// =============================================================================
void Camera::initOrthographic() {
    GLfloat l = -screenX / 2.0f;
    GLfloat r =  screenX / 2.0f;
    GLfloat b = -screenY / 2.0f;
    GLfloat t =  screenY / 2.0f;
    GLfloat n = -1.0f;
    GLfloat f =  1.0f;

    projVec[0] = 2.0f / (r - l);
    projVec[1] = 0.0f;
    projVec[2] = 0.0f;
    projVec[3] = 0.0f;
    projVec[4] = 0.0f;
    projVec[5] = 2.0f / (t - b);
    projVec[6] = 0.0f;
    projVec[7] = 0.0f;
    projVec[8] = 0.0f;
    projVec[9] = 0.0f;
    projVec[10] = -2.0f / (f - n);
    projVec[11] = 0.0f;
    projVec[12] = -(r + l) / (r - l);
    projVec[13] = -(t + b) / (t - b);
    projVec[14] = -(f + n) / (f - n);
    projVec[15] = 1.0f;
}

// =============================================================================
// Initialize Perspective Projection Matrix
// =============================================================================
void Camera::initPerspective() {
    GLfloat l = -screenX / 2.0f;
    GLfloat r =  screenX / 2.0f;
    GLfloat b = -screenY / 2.0f;
    GLfloat t =  screenY / 2.0f;
    GLfloat n = 1.0f;
    GLfloat f = 10.0f;

    projVec[0] = 2.0f * n / (r - l);
    projVec[1] = 0.0f;
    projVec[2] = 0.0f;
    projVec[3] = 0.0f;
    projVec[4] = 0.0f;
    projVec[5] = 2.0f * n / (t - b);
    projVec[6] = 0.0f;
    projVec[7] = 0.0f;
    projVec[8] = (r + l) / (r - l);
    projVec[9] = (t + b) / (t - b);
    projVec[10] = -(f + n) / (f - n);
    projVec[11] = -1.0f;
    projVec[12] = 0.0f;
    projVec[13] = 0.0f;
    projVec[14] = -(2 * f * n) / (f - n);
    projVec[15] = 0.0f;
}

// =============================================================================
// Initialize View Matrix
// =============================================================================
void Camera::initView() {
    viewVec[0] = 1.0f; // scale x
    viewVec[1] = 0.0f;
    viewVec[2] = 0.0f;
    viewVec[3] = 0.0f;
    viewVec[4] = 0.0f;
    viewVec[5] = 1.0f; // scale y
    viewVec[6] = 0.0f;
    viewVec[7] = 0.0f;
    viewVec[8] = 0.0f;
    viewVec[9] = 0.0f;
    viewVec[10] = 1.0f; // scale z
    viewVec[11] = 0.0f;
    viewVec[12] = INIT_POS_X; // x
    viewVec[13] = INIT_POS_Y; // y
    viewVec[14] = INIT_POS_Z; // z
    viewVec[15] = 1.0f;
}

// =============================================================================
// Update View Matrix
// =============================================================================
void Camera::moveView(float x, float y) {
    this->x = x;
    this->y = y;

    viewVec[12] = -x;
    viewVec[13] = -y;
}
// =============================================================================
// Update View Matrix
// =============================================================================
void Camera::moveView(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;

    viewVec[12] = -x;
    viewVec[13] = -y;
    viewVec[14] = -z;
}

#endif // CAMERA_H
