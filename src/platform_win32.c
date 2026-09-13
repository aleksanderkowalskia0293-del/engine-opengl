#include "platform.h"
#include "gl.h"
#include "wgl_ext.h"
#include <stdio.h>
#include <stdlib.h>

struct Platform {
    HWND hwnd;
    HDC hdc;
    HGLRC glrc;
    int width, height;
    int center_x, center_y;
};

static Platform *g_platform = NULL;
static InputState *g_input = NULL;

static void recenter_cursor(Platform *p) {
    POINT pt = { p->center_x, p->center_y };
    ClientToScreen(p->hwnd, &pt);
    SetCursorPos(pt.x, pt.y);
}

static void set_key(InputState *in, WPARAM vk, bool pressed) {
    switch (vk) {
        case 'W': case VK_UP:    in->forward = pressed; break;
        case 'S': case VK_DOWN:  in->back    = pressed; break;
        case 'A': case VK_LEFT:  in->left    = pressed; break;
        case 'D': case VK_RIGHT: in->right   = pressed; break;
        case VK_SPACE:           in->up      = pressed; break;
        case VK_SHIFT:           in->down    = pressed; break;
        default: break;
    }
}

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CLOSE:
            if (g_input) g_input->quit_requested = true;
            return 0;
        case WM_KEYDOWN:
            if (wp == VK_ESCAPE && g_input) g_input->quit_requested = true;
            if (g_input) set_key(g_input, wp, true);
            return 0;
        case WM_KEYUP:
            if (g_input) set_key(g_input, wp, false);
            return 0;
        case WM_MOUSEMOVE:
            if (g_platform && g_input && GetForegroundWindow() == hwnd) {
                int x = (short)(lp & 0xFFFF);
                int y = (short)((lp >> 16) & 0xFFFF);
                if (x != g_platform->center_x || y != g_platform->center_y) {
                    g_input->mouse_dx += (float)(x - g_platform->center_x);
                    g_input->mouse_dy += (float)(y - g_platform->center_y);
                    recenter_cursor(g_platform);
                }
            }
            return 0;
        case WM_SIZE:
            if (g_platform) {
                g_platform->width = LOWORD(lp);
                g_platform->height = HIWORD(lp);
                g_platform->center_x = g_platform->width / 2;
                g_platform->center_y = g_platform->height / 2;
            }
            return 0;
        case WM_DESTROY:
            return 0;
        default:
            return DefWindowProcA(hwnd, msg, wp, lp);
    }
}

Platform *platform_create_window(const char *title, int width, int height) {
    Platform *p = calloc(1, sizeof(Platform));
    if (!p) return NULL;
    p->width = width;
    p->height = height;
    p->center_x = width / 2;
    p->center_y = height / 2;

    HINSTANCE hinst = GetModuleHandleA(NULL);

    WNDCLASSEXA wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = hinst;
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.lpszClassName = "EngineWindowClass";
    if (!RegisterClassExA(&wc)) {
        fprintf(stderr, "platform_win32: RegisterClassEx failed\n");
        free(p);
        return NULL;
    }

    RECT rect = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rect, style, FALSE);

    p->hwnd = CreateWindowExA(0, wc.lpszClassName, title, style,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               rect.right - rect.left, rect.bottom - rect.top,
                               NULL, NULL, hinst, NULL);
    if (!p->hwnd) {
        fprintf(stderr, "platform_win32: CreateWindowEx failed\n");
        free(p);
        return NULL;
    }

    p->hdc = GetDC(p->hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(p->hdc, &pfd);
    if (!pf || !SetPixelFormat(p->hdc, pf, &pfd)) {
        fprintf(stderr, "platform_win32: SetPixelFormat failed\n");
        DestroyWindow(p->hwnd);
        free(p);
        return NULL;
    }

    HGLRC temp_ctx = wglCreateContext(p->hdc);
    wglMakeCurrent(p->hdc, temp_ctx);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (wglCreateContextAttribsARB) {
        int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        p->glrc = wglCreateContextAttribsARB(p->hdc, NULL, attribs);
    }

    if (p->glrc) {
        wglMakeCurrent(p->hdc, p->glrc);
        wglDeleteContext(temp_ctx);
    } else {
        fprintf(stderr, "platform_win32: falling back to a legacy GL context "
                        "(wglCreateContextAttribsARB unavailable)\n");
        p->glrc = temp_ctx;
    }

    ShowWindow(p->hwnd, SW_SHOW);
    UpdateWindow(p->hwnd);

    ShowCursor(FALSE);
    recenter_cursor(p);

    g_platform = p;
    return p;
}

void platform_destroy_window(Platform *p) {
    if (!p) return;
    ShowCursor(TRUE);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(p->glrc);
    ReleaseDC(p->hwnd, p->hdc);
    DestroyWindow(p->hwnd);
    if (g_platform == p) g_platform = NULL;
    free(p);
}

void platform_poll_events(Platform *p, InputState *input) {
    input->mouse_dx = 0.0f;
    input->mouse_dy = 0.0f;
    g_platform = p;
    g_input = input;

    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void platform_swap_buffers(Platform *p) {
    SwapBuffers(p->hdc);
}

void platform_get_size(Platform *p, int *width, int *height) {
    *width = p->width;
    *height = p->height;
}

double platform_get_time_seconds(void) {
    static LARGE_INTEGER freq;
    static bool have_freq = false;
    if (!have_freq) {
        QueryPerformanceFrequency(&freq);
        have_freq = true;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (double)now.QuadPart / (double)freq.QuadPart;
}

void *platform_gl_get_proc_address(const char *name) {
    void *fn = (void *)wglGetProcAddress(name);
    if (fn == NULL || fn == (void *)1 || fn == (void *)2 || fn == (void *)3 || fn == (void *)-1) {
        HMODULE mod = GetModuleHandleA("opengl32.dll");
        fn = (void *)GetProcAddress(mod, name);
    }
    return fn;
}
