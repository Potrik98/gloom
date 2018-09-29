#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include "handout/mesh.hpp"

/*
 * Class for handling vertex array objects
 * with an arbitrary amount of vertex attributes.
 * Handles vao generation, binding of index and vertex buffers,
 * and rendering of the vao.
 * 
 * Example usage:
 * VertexArrayObject vao(vertex_count);
 * vao.indexArray(indices, index_count) // binds the indices to the vao
 *   ->vertexArray(vertices, 3, 0); // binds vertices to location 0, with component count 3
 * 
 * vao.render(); // renders the vao.
 */
class VertexArrayObject {
public:
    VertexArrayObject(const unsigned int& vertex_count) {
        m_vertex_count = vertex_count;
        m_index_count = 0;
        glGenVertexArrays(1, &m_id);
    }

    VertexArrayObject() {} // Default constructor required for simpler task initialization

    /*
     * Binds an index array to the vao.
     * The vao will use this index array when rendering.
     */
    VertexArrayObject* indexArray(const unsigned int* indices,
                                  const unsigned int& index_count) {
        m_index_count = index_count;
        glBindVertexArray(m_id);

        unsigned int idx_buf_id;
        glGenBuffers(1, &idx_buf_id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(int), indices, GL_STATIC_DRAW);

        return this;
    }

    /*
     * Binds an index array to the vao.
     * The vao will use this index array when rendering.
     */
    VertexArrayObject* indexArray(const std::vector<unsigned int>& indices) {
        return this->indexArray(&indices[0], indices.size());
    }

    /*
     * Binds a vertex array to the vao in the given location.
     * Takes in the vertex data, component count and location.
     */
    VertexArrayObject* vertexArray(const float* vertices,
                                   const unsigned int& component_count,
                                   const unsigned int& location) {
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

        // Set the attrib pointer for the location
        glVertexAttribPointer(
            location,
            component_count,
            GL_FLOAT, // float data
            GL_FALSE,
            0,
            0
        );

        // Enable attributes for the location
        glEnableVertexAttribArray(location);

        return this;
    }

    /*
     * Binds a vertex array to the vao in the given location.
     * Takes in the vertex data, component count and location.
     */
    VertexArrayObject* vertexArray(const std::vector<float3>& vertices,
                                   const unsigned int& location) {
        const unsigned int component_count = 3;
        const unsigned int vertex_count = vertices.size();
        float vertex_data[vertex_count * component_count];
        int index = 0;
        for (float3 v : vertices) {
            vertex_data[index++] = v.x;
            vertex_data[index++] = v.y;
            vertex_data[index++] = v.z;
        }
        return this->vertexArray(vertex_data, component_count, location);
    }

    /*
     * Binds a vertex array to the vao in the given location.
     * Takes in the vertex data, component count and location.
     */
    VertexArrayObject* vertexArray(const std::vector<float4>& vertices,
                                   const unsigned int& location) {
        const unsigned int component_count = 4;
        const unsigned int vertex_count = vertices.size();
        float vertex_data[vertex_count * component_count];
        int index = 0;
        for (float4 v : vertices) {
            vertex_data[index++] = v.x;
            vertex_data[index++] = v.y;
            vertex_data[index++] = v.z;
            vertex_data[index++] = v.w;
        }
        return this->vertexArray(vertex_data, component_count, location);
    }

    /*
     * Render the vao
     */
    void render() {
        // TODO: enable rendering of VAOs without index buffers
        glBindVertexArray(m_id);
        glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
    }

    unsigned int getVertexCount() { return m_vertex_count; }
private:
    GLuint m_id;
    unsigned int m_vertex_count;
    unsigned int m_index_count;
};

VertexArrayObject fromMesh(const Mesh& mesh) {
    const unsigned int vertex_count = mesh.vertices.size();
    VertexArrayObject result(vertex_count);
    result.indexArray(mesh.indices)
         ->vertexArray(mesh.vertices, 0)
         ->vertexArray(mesh.colours, 1);
    if (mesh.hasNormals) {
        result.vertexArray(mesh.normals, 2);
    }
    return result;
}
