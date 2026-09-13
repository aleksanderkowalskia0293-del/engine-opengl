#ifndef ENGINE_MATH3D_H
#define ENGINE_MATH3D_H

#include <math.h>

typedef struct { float x, y, z; } vec3;
typedef struct { float m[16]; } mat4;

static inline vec3 vec3_make(float x, float y, float z) { vec3 v = {x, y, z}; return v; }
static inline vec3 vec3_add(vec3 a, vec3 b) { return vec3_make(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline vec3 vec3_sub(vec3 a, vec3 b) { return vec3_make(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline vec3 vec3_scale(vec3 a, float s) { return vec3_make(a.x * s, a.y * s, a.z * s); }
static inline float vec3_dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_make(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
static inline float vec3_length(vec3 a) { return sqrtf(vec3_dot(a, a)); }
static inline vec3 vec3_normalize(vec3 a) {
    float len = vec3_length(a);
    if (len < 1e-8f) return vec3_make(0, 0, 0);
    return vec3_scale(a, 1.0f / len);
}

static inline mat4 mat4_identity(void) {
    mat4 r = {{0}};
    r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
    return r;
}

static inline mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 r;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            r.m[col * 4 + row] = sum;
        }
    }
    return r;
}

static inline mat4 mat4_translate(vec3 t) {
    mat4 r = mat4_identity();
    r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
    return r;
}

static inline mat4 mat4_scale(vec3 s) {
    mat4 r = mat4_identity();
    r.m[0] = s.x; r.m[5] = s.y; r.m[10] = s.z;
    return r;
}

static inline mat4 mat4_rotate_y(float radians) {
    mat4 r = mat4_identity();
    float c = cosf(radians), s = sinf(radians);
    r.m[0] = c;  r.m[8]  = s;
    r.m[2] = -s; r.m[10] = c;
    return r;
}

static inline mat4 mat4_rotate_x(float radians) {
    mat4 r = mat4_identity();
    float c = cosf(radians), s = sinf(radians);
    r.m[5] = c;  r.m[9]  = -s;
    r.m[6] = s;  r.m[10] = c;
    return r;
}

static inline mat4 mat4_perspective(float fovy_radians, float aspect, float znear, float zfar) {
    mat4 r = {{0}};
    float f = 1.0f / tanf(fovy_radians * 0.5f);
    r.m[0] = f / aspect;
    r.m[5] = f;
    r.m[10] = (zfar + znear) / (znear - zfar);
    r.m[11] = -1.0f;
    r.m[14] = (2.0f * zfar * znear) / (znear - zfar);
    return r;
}

static inline mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up) {
    vec3 f = vec3_normalize(vec3_sub(center, eye));
    vec3 s = vec3_normalize(vec3_cross(f, up));
    vec3 u = vec3_cross(s, f);

    mat4 r = mat4_identity();
    r.m[0] = s.x; r.m[4] = s.y; r.m[8]  = s.z;
    r.m[1] = u.x; r.m[5] = u.y; r.m[9]  = u.z;
    r.m[2] = -f.x; r.m[6] = -f.y; r.m[10] = -f.z;
    r.m[12] = -vec3_dot(s, eye);
    r.m[13] = -vec3_dot(u, eye);
    r.m[14] = vec3_dot(f, eye);
    return r;
}

#endif
