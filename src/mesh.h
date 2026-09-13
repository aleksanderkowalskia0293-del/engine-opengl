#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include "gl.h"
#include <stddef.h>

typedef struct {
    GLuint vao, vbo, ebo;
    GLsizei index_count;
} Mesh;

Mesh mesh_create(const float *vertices, size_t vertex_count,
                  const unsigned int *indices, size_t index_count);

Mesh mesh_create_cube(float size);

Mesh mesh_create_plane(float size, float tex_repeat);

void mesh_draw(const Mesh *mesh);
void mesh_destroy(Mesh *mesh);

#endif
