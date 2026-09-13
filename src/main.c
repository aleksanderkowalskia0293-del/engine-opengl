#include "platform.h"
#include "gl.h"
#include "math3d.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "audio.h"
#include <stdio.h>

#define PI 3.14159265358979323846f

int main(void) {
    Platform *platform = platform_create_window("Basic C/OpenGL Engine", 1280, 720);
    if (!platform) {
        fprintf(stderr, "main: failed to create window/GL context\n");
        return 1;
    }

    if (!gl_load_functions()) {
        fprintf(stderr, "main: failed to load required OpenGL 3.3 functions\n");
        platform_destroy_window(platform);
        return 1;
    }

    printf("GL vendor:   %s\n", glGetString(GL_VENDOR));
    printf("GL renderer: %s\n", glGetString(GL_RENDERER));
    printf("GL version:  %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(0.53f, 0.72f, 0.90f, 1.0f);

    GLuint program = shader_load_program("shaders/basic.vert", "shaders/basic.frag");
    if (!program) {
        fprintf(stderr, "main: shader load failed (run the binary from the repo root)\n");
        platform_destroy_window(platform);
        return 1;
    }

    GLuint texture = texture_load_png("assets/checker.png");
    if (!texture) {
        fprintf(stderr, "main: texture load failed (run `make assets` to generate it)\n");
        platform_destroy_window(platform);
        return 1;
    }

    Mesh cube = mesh_create_cube(1.0f);
    Mesh ground = mesh_create_plane(20.0f, 10.0f);

    GLint loc_model = glGetUniformLocation(program, "uModel");
    GLint loc_view = glGetUniformLocation(program, "uView");
    GLint loc_proj = glGetUniformLocation(program, "uProj");
    GLint loc_tex = glGetUniformLocation(program, "uTexture");
    GLint loc_light = glGetUniformLocation(program, "uLightDir");

    Camera camera = camera_init(vec3_make(0.0f, 1.5f, 5.0f));
    InputState input = {0};

    audio_play_wav("assets/beep.wav");

    double last_time = platform_get_time_seconds();
    float cube_angle = 0.0f;

    while (!input.quit_requested) {
        platform_poll_events(platform, &input);

        double now = platform_get_time_seconds();
        float dt = (float)(now - last_time);
        last_time = now;
        if (dt > 0.1f) dt = 0.1f;

        camera_update(&camera, &input, dt);

        cube_angle += dt * 0.8f;
        if (cube_angle > 2.0f * PI) {
            cube_angle -= 2.0f * PI;
            audio_play_wav("assets/beep.wav");
        }

        int width, height;
        platform_get_size(platform, &width, &height);
        if (height <= 0) height = 1;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = camera_view_matrix(&camera);
        mat4 proj = mat4_perspective(camera.fov_radians, (float)width / (float)height, 0.1f, 100.0f);

        glUseProgram(program);
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(loc_proj, 1, GL_FALSE, proj.m);
        glUniform3f(loc_light, -0.4f, -1.0f, -0.3f);
        glUniform1i(loc_tex, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        mat4 ground_model = mat4_identity();
        glUniformMatrix4fv(loc_model, 1, GL_FALSE, ground_model.m);
        mesh_draw(&ground);

        mat4 cube_model = mat4_mul(mat4_translate(vec3_make(0.0f, 1.0f, 0.0f)), mat4_rotate_y(cube_angle));
        glUniformMatrix4fv(loc_model, 1, GL_FALSE, cube_model.m);
        mesh_draw(&cube);

        platform_swap_buffers(platform);
    }

    mesh_destroy(&cube);
    mesh_destroy(&ground);
    glDeleteTextures(1, &texture);
    glDeleteProgram(program);
    platform_destroy_window(platform);
    return 0;
}
