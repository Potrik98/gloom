#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

class VertexArrayObject {
public:
    VertexArrayObject(const unsigned int& vertex_count) {
        m_vertex_count = vertex_count;
        glGenVertexArrays(1, &m_id);
    }

    VertexArrayObject* indexArray(const int* indices,
                                  const unsigned int& index_count) {
        glBindVertexArray(m_id);

        unsigned int idx_buf_id;
        glGenBuffers(1, &idx_buf_id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(int), indices, GL_STATIC_DRAW);

        return this;
    }

    VertexArrayObject* vertexArray(const float* vertices,
                                   const int& component_count,
                                   const int& location) {
        glBindVertexArray(m_id);

        unsigned int vtx_buf_id;
        glGenBuffers(1, &vtx_buf_id);

        glBindBuffer(GL_ARRAY_BUFFER, vtx_buf_id);
        glBufferData(
            GL_ARRAY_BUFFER,
            m_vertex_count * component_count * sizeof(float),
            vertices,
            GL_STATIC_DRAW
        );
        glVertexAttribPointer(
            location,
            component_count,
            GL_FLOAT, // float data
            GL_FALSE,
            0,
            0
        );
        glEnableVertexAttribArray(location);

        return this;
    }

    GLuint id() { return m_id; }
private:
    GLuint m_id;
    unsigned int m_vertex_count;
};
