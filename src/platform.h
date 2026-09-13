#ifndef ENGINE_PLATFORM_H
#define ENGINE_PLATFORM_H

#include <stdbool.h>

typedef struct Platform Platform;

typedef struct {
    bool forward, back, left, right, up, down;
    bool quit_requested;
    float mouse_dx, mouse_dy;
} InputState;

Platform *platform_create_window(const char *title, int width, int height);
void platform_destroy_window(Platform *p);

void platform_poll_events(Platform *p, InputState *input);

void platform_swap_buffers(Platform *p);
void platform_get_size(Platform *p, int *width, int *height);
double platform_get_time_seconds(void);

void *platform_gl_get_proc_address(const char *name);

#endif
