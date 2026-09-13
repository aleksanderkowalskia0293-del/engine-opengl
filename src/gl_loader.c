#include "gl.h"
#include "platform.h"
#include <stdio.h>

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

PFNGLGENTEXTURESPROC glGenTextures;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLTEXIMAGE2DPROC glTexImage2D;
PFNGLTEXPARAMETERIPROC glTexParameteri;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLDELETETEXTURESPROC glDeleteTextures;

static void *load(const char *name) {
    void *fn = platform_gl_get_proc_address(name);
    if (!fn) {
        fprintf(stderr, "gl_loader: failed to resolve %s\n", name);
    }
    return fn;
}

#define LOAD(var, name) do { \
        var = (__typeof__(var))load(name); \
        if (!var) ok = 0; \
    } while (0)

int gl_load_functions(void) {
    int ok = 1;

    LOAD(glGenVertexArrays, "glGenVertexArrays");
    LOAD(glBindVertexArray, "glBindVertexArray");
    LOAD(glDeleteVertexArrays, "glDeleteVertexArrays");
    LOAD(glGenBuffers, "glGenBuffers");
    LOAD(glBindBuffer, "glBindBuffer");
    LOAD(glBufferData, "glBufferData");
    LOAD(glDeleteBuffers, "glDeleteBuffers");
    LOAD(glVertexAttribPointer, "glVertexAttribPointer");
    LOAD(glEnableVertexAttribArray, "glEnableVertexAttribArray");

    LOAD(glCreateShader, "glCreateShader");
    LOAD(glShaderSource, "glShaderSource");
    LOAD(glCompileShader, "glCompileShader");
    LOAD(glGetShaderiv, "glGetShaderiv");
    LOAD(glGetShaderInfoLog, "glGetShaderInfoLog");
    LOAD(glDeleteShader, "glDeleteShader");
    LOAD(glCreateProgram, "glCreateProgram");
    LOAD(glAttachShader, "glAttachShader");
    LOAD(glLinkProgram, "glLinkProgram");
    LOAD(glGetProgramiv, "glGetProgramiv");
    LOAD(glGetProgramInfoLog, "glGetProgramInfoLog");
    LOAD(glUseProgram, "glUseProgram");
    LOAD(glDeleteProgram, "glDeleteProgram");
    LOAD(glGetUniformLocation, "glGetUniformLocation");
    LOAD(glUniform1i, "glUniform1i");
    LOAD(glUniform1f, "glUniform1f");
    LOAD(glUniform3f, "glUniform3f");
    LOAD(glUniformMatrix4fv, "glUniformMatrix4fv");

    LOAD(glGenTextures, "glGenTextures");
    LOAD(glBindTexture, "glBindTexture");
    LOAD(glTexImage2D, "glTexImage2D");
    LOAD(glTexParameteri, "glTexParameteri");
    LOAD(glGenerateMipmap, "glGenerateMipmap");
    LOAD(glActiveTexture, "glActiveTexture");
    LOAD(glDeleteTextures, "glDeleteTextures");

    return ok;
}
