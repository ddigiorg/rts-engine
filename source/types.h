#ifndef TYPES_H
#define TYPES_H

// third party includes
#include <GL/glew.h>

// type definitions
typedef union vec2i_u {
    GLint raw[2];
    struct {
        GLint x, y;
    };
} vec2i_t;

typedef union vec2f_u {
    GLfloat raw[2];
    struct {
        GLfloat x, y;
    };
} vec2f_t;

typedef union vec3f_u {
    GLfloat raw[3];
    struct {
        GLfloat x, y, z;
    };
} vec3f_t;

typedef union mat4x4f_u {
    GLfloat flat[16];
    GLfloat grid[4][4];
    struct{
        GLfloat m00, m01, m02, m03,
                m10, m11, m12, m13,
                m20, m21, m22, m23,
                m30, m31, m32, m33;
    };
} mat4x4f_t;

#endif // TYPES_H