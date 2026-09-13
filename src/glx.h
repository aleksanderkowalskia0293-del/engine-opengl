#ifndef ENGINE_GLX_H
#define ENGINE_GLX_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "gl.h"

typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXDrawable;
typedef XID GLXWindow;
typedef struct __GLXFBConfigRec *GLXFBConfig;

#define GLX_RGBA_TYPE                       0x8014
#define GLX_DRAWABLE_TYPE                   0x8010
#define GLX_RENDER_TYPE                     0x8011
#define GLX_X_RENDERABLE                    0x8012
#define GLX_WINDOW_BIT                      0x00000001
#define GLX_RGBA_BIT                        0x00000001
#define GLX_DOUBLEBUFFER                    5
#define GLX_RED_SIZE                        8
#define GLX_GREEN_SIZE                      9
#define GLX_BLUE_SIZE                       10
#define GLX_ALPHA_SIZE                      11
#define GLX_DEPTH_SIZE                      12
#define GLX_STENCIL_SIZE                    13

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
#define GLX_CONTEXT_FLAGS_ARB               0x2094
#define GLX_CONTEXT_PROFILE_MASK_ARB        0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB    0x00000001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

extern Bool glXQueryVersion(Display *dpy, int *major, int *minor);
extern GLXFBConfig *glXChooseFBConfig(Display *dpy, int screen, const int *attrib_list, int *nelements);
extern XVisualInfo *glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config);
extern int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
extern GLXContext glXCreateContext(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
extern GLXContext glXCreateContextAttribsARB(Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);
extern Bool glXMakeCurrent(Display *dpy, GLXDrawable drawable, GLXContext ctx);
extern void glXSwapBuffers(Display *dpy, GLXDrawable drawable);
extern void glXDestroyContext(Display *dpy, GLXContext ctx);
extern Bool glXIsDirect(Display *dpy, GLXContext ctx);
extern void (*glXGetProcAddressARB(const GLubyte *procName))(void);

#endif
