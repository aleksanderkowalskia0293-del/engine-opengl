#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "math3d.h"
#include "platform.h"

typedef struct {
    vec3 position;
    vec3 front, up, right;
    float yaw, pitch;
    float fov_radians;
    float move_speed;
    float mouse_sensitivity;
} Camera;

Camera camera_init(vec3 start_position);

void camera_update(Camera *cam, const InputState *input, float dt);

mat4 camera_view_matrix(const Camera *cam);

#endif
