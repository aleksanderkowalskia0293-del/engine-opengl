#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "shader: cannot open %s\n", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (size < 0) { fclose(f); return NULL; }

    char *buf = malloc((size_t)size + 1);
    if (!buf) { fclose(f); return NULL; }

    size_t read = fread(buf, 1, (size_t)size, f);
    fclose(f);
    buf[read] = '\0';
    return buf;
}

static GLuint compile_stage(GLenum type, const char *source, const char *path) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        GLsizei len = 0;
        glGetShaderInfoLog(shader, sizeof(log), &len, log);
        fprintf(stderr, "shader: failed to compile %s:\n%.*s\n", path, (int)len, log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint shader_load_program(const char *vertex_path, const char *fragment_path) {
    char *vsrc = read_file(vertex_path);
    char *fsrc = read_file(fragment_path);
    if (!vsrc || !fsrc) {
        free(vsrc);
        free(fsrc);
        return 0;
    }

    GLuint vs = compile_stage(GL_VERTEX_SHADER, vsrc, vertex_path);
    GLuint fs = compile_stage(GL_FRAGMENT_SHADER, fsrc, fragment_path);
    free(vsrc);
    free(fsrc);
    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        GLsizei len = 0;
        glGetProgramInfoLog(program, sizeof(log), &len, log);
        fprintf(stderr, "shader: failed to link %s + %s:\n%.*s\n",
                vertex_path, fragment_path, (int)len, log);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}
