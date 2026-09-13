#define _POSIX_C_SOURCE 199309L
#include "platform.h"
#include "glx.h"
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Platform {
    Display *dpy;
    Window win;
    GLXContext ctx;
    Atom wm_delete;
    int width, height;
    int center_x, center_y;
    bool have_focus;
};

Platform *platform_create_window(const char *title, int width, int height) {
    Platform *p = calloc(1, sizeof(Platform));
    if (!p) return NULL;
    p->width = width;
    p->height = height;

    p->dpy = XOpenDisplay(NULL);
    if (!p->dpy) {
        fprintf(stderr, "platform_x11: cannot open X display (is DISPLAY set / an X server running?)\n");
        free(p);
        return NULL;
    }
    int screen = DefaultScreen(p->dpy);

    int fb_attribs[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        None
    };
    int fb_count = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(p->dpy, screen, fb_attribs, &fb_count);
    if (!fbc || fb_count == 0) {
        fprintf(stderr, "platform_x11: no matching GLX framebuffer config\n");
        XCloseDisplay(p->dpy);
        free(p);
        return NULL;
    }
    GLXFBConfig chosen = fbc[0];
    XFree(fbc);

    XVisualInfo *vi = glXGetVisualFromFBConfig(p->dpy, chosen);
    if (!vi) {
        fprintf(stderr, "platform_x11: glXGetVisualFromFBConfig failed\n");
        XCloseDisplay(p->dpy);
        free(p);
        return NULL;
    }

    Window root = RootWindow(p->dpy, vi->screen);
    XSetWindowAttributes swa;
    memset(&swa, 0, sizeof(swa));
    swa.colormap = XCreateColormap(p->dpy, root, vi->visual, AllocNone);
    swa.event_mask = KeyPressMask | KeyReleaseMask | PointerMotionMask |
                      StructureNotifyMask | FocusChangeMask;

    p->win = XCreateWindow(p->dpy, root, 0, 0, (unsigned)width, (unsigned)height, 0,
                            vi->depth, InputOutput, vi->visual,
                            CWColormap | CWEventMask, &swa);
    XFree(vi);

    XStoreName(p->dpy, p->win, title);
    p->wm_delete = XInternAtom(p->dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(p->dpy, p->win, &p->wm_delete, 1);
    XMapWindow(p->dpy, p->win);

    int ctx_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };
    p->ctx = glXCreateContextAttribsARB(p->dpy, chosen, NULL, True, ctx_attribs);
    if (!p->ctx) {
        fprintf(stderr, "platform_x11: failed to create GL 3.3 core context\n");
        XDestroyWindow(p->dpy, p->win);
        XCloseDisplay(p->dpy);
        free(p);
        return NULL;
    }
    XSync(p->dpy, False);
    glXMakeCurrent(p->dpy, p->win, p->ctx);

    Pixmap blank_pm = XCreateBitmapFromData(p->dpy, p->win, (char[]){0}, 1, 1);
    XColor dummy;
    memset(&dummy, 0, sizeof(dummy));
    Cursor blank_cursor = XCreatePixmapCursor(p->dpy, blank_pm, blank_pm, &dummy, &dummy, 0, 0);
    XDefineCursor(p->dpy, p->win, blank_cursor);
    XFreeCursor(p->dpy, blank_cursor);
    XFreePixmap(p->dpy, blank_pm);

    p->center_x = width / 2;
    p->center_y = height / 2;
    XWarpPointer(p->dpy, None, p->win, 0, 0, 0, 0, p->center_x, p->center_y);
    XSync(p->dpy, False);

    return p;
}

void platform_destroy_window(Platform *p) {
    if (!p) return;
    glXMakeCurrent(p->dpy, None, NULL);
    glXDestroyContext(p->dpy, p->ctx);
    XDestroyWindow(p->dpy, p->win);
    XCloseDisplay(p->dpy);
    free(p);
}

static void set_key(InputState *in, KeySym sym, bool pressed) {
    switch (sym) {
        case XK_w: case XK_W: case XK_Up:    in->forward = pressed; break;
        case XK_s: case XK_S: case XK_Down:  in->back    = pressed; break;
        case XK_a: case XK_A: case XK_Left:  in->left    = pressed; break;
        case XK_d: case XK_D: case XK_Right: in->right   = pressed; break;
        case XK_space:                        in->up      = pressed; break;
        case XK_Shift_L: case XK_Shift_R:     in->down    = pressed; break;
        default: break;
    }
}

void platform_poll_events(Platform *p, InputState *input) {
    input->mouse_dx = 0.0f;
    input->mouse_dy = 0.0f;

    XEvent ev;
    while (XPending(p->dpy) > 0) {
        XNextEvent(p->dpy, &ev);
        switch (ev.type) {
            case ClientMessage:
                if ((Atom)ev.xclient.data.l[0] == p->wm_delete) {
                    input->quit_requested = true;
                }
                break;
            case KeyPress: {
                KeySym sym = XLookupKeysym(&ev.xkey, 0);
                if (sym == XK_Escape) input->quit_requested = true;
                set_key(input, sym, true);
                break;
            }
            case KeyRelease: {
                KeySym sym = XLookupKeysym(&ev.xkey, 0);
                set_key(input, sym, false);
                break;
            }
            case MotionNotify:
                if (ev.xmotion.x != p->center_x || ev.xmotion.y != p->center_y) {
                    input->mouse_dx += (float)(ev.xmotion.x - p->center_x);
                    input->mouse_dy += (float)(ev.xmotion.y - p->center_y);
                    XWarpPointer(p->dpy, None, p->win, 0, 0, 0, 0, p->center_x, p->center_y);
                }
                break;
            case ConfigureNotify:
                p->width = ev.xconfigure.width;
                p->height = ev.xconfigure.height;
                p->center_x = p->width / 2;
                p->center_y = p->height / 2;
                break;
            default:
                break;
        }
    }
}

void platform_swap_buffers(Platform *p) {
    glXSwapBuffers(p->dpy, p->win);
}

void platform_get_size(Platform *p, int *width, int *height) {
    *width = p->width;
    *height = p->height;
}

double platform_get_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

void *platform_gl_get_proc_address(const char *name) {
    return (void *)glXGetProcAddressARB((const GLubyte *)name);
}
