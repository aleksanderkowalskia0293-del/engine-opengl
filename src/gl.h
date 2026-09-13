#ifndef ENGINE_GL_H
#define ENGINE_GL_H

#include <stddef.h>

#ifdef _WIN32
  #define GLAPI __stdcall
#else
  #define GLAPI
#endif

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

#define GL_FALSE                       0
#define GL_TRUE                        1
#define GL_NO_ERROR                    0

#define GL_DEPTH_BUFFER_BIT            0x00000100
#define GL_COLOR_BUFFER_BIT            0x00004000

#define GL_TRIANGLES                   0x0004
#define GL_LINES                       0x0001

#define GL_BYTE                        0x1400
#define GL_UNSIGNED_BYTE               0x1401
#define GL_SHORT                       0x1402
#define GL_UNSIGNED_SHORT              0x1403
#define GL_INT                         0x1404
#define GL_UNSIGNED_INT                0x1405
#define GL_FLOAT                       0x1406

#define GL_DEPTH_TEST                  0x0B71
#define GL_CULL_FACE                   0x0B44
#define GL_BLEND                       0x0BE2
#define GL_FRONT                       0x0404
#define GL_BACK                        0x0405
#define GL_CCW                         0x0901
#define GL_CW                          0x0900

#define GL_SRC_ALPHA                   0x0302
#define GL_ONE_MINUS_SRC_ALPHA         0x0303

#define GL_ARRAY_BUFFER                0x8892
#define GL_ELEMENT_ARRAY_BUFFER        0x8893
#define GL_STATIC_DRAW                 0x88E4
#define GL_DYNAMIC_DRAW                0x88E8

#define GL_VERTEX_SHADER               0x8B31
#define GL_FRAGMENT_SHADER             0x8B30
#define GL_COMPILE_STATUS              0x8B81
#define GL_LINK_STATUS                 0x8B82
#define GL_INFO_LOG_LENGTH             0x8B84

#define GL_TEXTURE_2D                  0x0DE1
#define GL_TEXTURE0                    0x84C0
#define GL_TEXTURE_MIN_FILTER          0x2801
#define GL_TEXTURE_MAG_FILTER          0x2800
#define GL_TEXTURE_WRAP_S              0x2802
#define GL_TEXTURE_WRAP_T              0x2803
#define GL_NEAREST                     0x2600
#define GL_LINEAR                      0x2601
#define GL_LINEAR_MIPMAP_LINEAR        0x2703
#define GL_REPEAT                      0x2901
#define GL_RGB                         0x1907
#define GL_RGBA                        0x1908
#define GL_RGB8                        0x8051
#define GL_RGBA8                       0x8058
#define GL_UNPACK_ALIGNMENT            0x0CF5

#define GL_VENDOR                      0x1F00
#define GL_RENDERER                    0x1F01
#define GL_VERSION                     0x1F02
#define GL_SHADING_LANGUAGE_VERSION    0x8B8C

extern void GLAPI glClear(GLbitfield mask);
extern void GLAPI glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
extern void GLAPI glViewport(GLint x, GLint y, GLsizei w, GLsizei h);
extern void GLAPI glEnable(GLenum cap);
extern void GLAPI glDisable(GLenum cap);
extern void GLAPI glCullFace(GLenum mode);
extern void GLAPI glFrontFace(GLenum mode);
extern void GLAPI glBlendFunc(GLenum sfactor, GLenum dfactor);
extern void GLAPI glDrawArrays(GLenum mode, GLint first, GLsizei count);
extern void GLAPI glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
extern GLenum GLAPI glGetError(void);
extern const GLubyte * GLAPI glGetString(GLenum name);
extern void GLAPI glPixelStorei(GLenum pname, GLint param);

typedef void (GLAPI *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (GLAPI *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (GLAPI *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (GLAPI *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (GLAPI *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (GLAPI *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (GLAPI *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (GLAPI *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (GLAPI *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);

typedef GLuint (GLAPI *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (GLAPI *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
typedef void (GLAPI *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (GLAPI *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (GLAPI *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPI *PFNGLDELETESHADERPROC)(GLuint shader);
typedef GLuint (GLAPI *PFNGLCREATEPROGRAMPROC)(void);
typedef void (GLAPI *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAPI *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (GLAPI *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (GLAPI *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPI *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (GLAPI *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef GLint (GLAPI *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (GLAPI *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (GLAPI *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (GLAPI *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPI *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

typedef void (GLAPI *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (GLAPI *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (GLAPI *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPI *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAPI *PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (GLAPI *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (GLAPI *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;

int gl_load_functions(void);

#endif
