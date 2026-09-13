#include "mesh.h"

Mesh mesh_create(const float *vertices, size_t vertex_count,
                  const unsigned int *indices, size_t index_count) {
    Mesh mesh = {0};
    mesh.index_count = (GLsizei)index_count;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertex_count * 8 * sizeof(float)),
                 vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(index_count * sizeof(unsigned int)),
                 indices, GL_STATIC_DRAW);

    GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return mesh;
}

Mesh mesh_create_cube(float size) {
    float h = size * 0.5f;
    float v[] = {
        /* +X */  h,-h,-h, 1,0,0, 0,0,   h,h,-h, 1,0,0, 0,1,   h,h,h, 1,0,0, 1,1,   h,-h,h, 1,0,0, 1,0,
        /* -X */ -h,-h,h, -1,0,0, 0,0,  -h,h,h, -1,0,0, 0,1,  -h,h,-h,-1,0,0, 1,1, -h,-h,-h,-1,0,0, 1,0,
        /* +Y */ -h,h,-h,  0,1,0, 0,0,  -h,h,h,  0,1,0, 0,1,   h,h,h,  0,1,0, 1,1,  h,h,-h,  0,1,0, 1,0,
        /* -Y */ -h,-h,h,  0,-1,0,0,0,  -h,-h,-h,0,-1,0,0,1,   h,-h,-h,0,-1,0,1,1,  h,-h,h,  0,-1,0,1,0,
        /* +Z */ -h,-h,h,  0,0,1, 0,0,   h,-h,h,  0,0,1, 1,0,   h,h,h,  0,0,1, 1,1, -h,h,h,   0,0,1, 0,1,
        /* -Z */  h,-h,-h, 0,0,-1,0,0,  -h,-h,-h,0,0,-1,1,0,  -h,h,-h, 0,0,-1,1,1,  h,h,-h,  0,0,-1,0,1,
    };
    unsigned int idx[36];
    for (unsigned int f = 0; f < 6; f++) {
        unsigned int base = f * 4;
        idx[f * 6 + 0] = base + 0; idx[f * 6 + 1] = base + 1; idx[f * 6 + 2] = base + 2;
        idx[f * 6 + 3] = base + 0; idx[f * 6 + 4] = base + 2; idx[f * 6 + 5] = base + 3;
    }
    return mesh_create(v, sizeof(v) / (8 * sizeof(float)), idx, 36);
}

Mesh mesh_create_plane(float size, float tex_repeat) {
    float h = size * 0.5f;
    float v[] = {
        -h, 0, -h,  0,1,0,  0,0,
        -h, 0,  h,  0,1,0,  0,tex_repeat,
         h, 0,  h,  0,1,0,  tex_repeat,tex_repeat,
         h, 0, -h,  0,1,0,  tex_repeat,0,
    };
    unsigned int idx[] = {0, 1, 2, 0, 2, 3};
    return mesh_create(v, 4, idx, 6);
}

void mesh_draw(const Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void mesh_destroy(Mesh *mesh) {
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
    mesh->vao = mesh->vbo = mesh->ebo = 0;
    mesh->index_count = 0;
}
