#include "camera.h"

#define PI 3.14159265358979323846f
#define PITCH_LIMIT (89.0f * PI / 180.0f)

static void update_vectors(Camera *cam) {
    vec3 world_up = vec3_make(0.0f, 1.0f, 0.0f);
    vec3 front;
    front.x = cosf(cam->pitch) * cosf(cam->yaw);
    front.y = sinf(cam->pitch);
    front.z = cosf(cam->pitch) * sinf(cam->yaw);
    cam->front = vec3_normalize(front);
    cam->right = vec3_normalize(vec3_cross(cam->front, world_up));
    cam->up = vec3_normalize(vec3_cross(cam->right, cam->front));
}

Camera camera_init(vec3 start_position) {
    Camera cam = {0};
    cam.position = start_position;
    cam.yaw = -PI * 0.5f;
    cam.pitch = 0.0f;
    cam.fov_radians = 60.0f * PI / 180.0f;
    cam.move_speed = 4.0f;
    cam.mouse_sensitivity = 0.0025f;
    update_vectors(&cam);
    return cam;
}

void camera_update(Camera *cam, const InputState *input, float dt) {
    cam->yaw += input->mouse_dx * cam->mouse_sensitivity;
    cam->pitch -= input->mouse_dy * cam->mouse_sensitivity;
    if (cam->pitch > PITCH_LIMIT) cam->pitch = PITCH_LIMIT;
    if (cam->pitch < -PITCH_LIMIT) cam->pitch = -PITCH_LIMIT;
    update_vectors(cam);

    float distance = cam->move_speed * dt;
    if (input->forward) cam->position = vec3_add(cam->position, vec3_scale(cam->front, distance));
    if (input->back)    cam->position = vec3_sub(cam->position, vec3_scale(cam->front, distance));
    if (input->right)   cam->position = vec3_add(cam->position, vec3_scale(cam->right, distance));
    if (input->left)    cam->position = vec3_sub(cam->position, vec3_scale(cam->right, distance));
    if (input->up)       cam->position.y += distance;
    if (input->down)     cam->position.y -= distance;
}

mat4 camera_view_matrix(const Camera *cam) {
    return mat4_look_at(cam->position, vec3_add(cam->position, cam->front), cam->up);
}
