#pragma once

#include "vao.hpp"
#include "math.h"

namespace geometry {
    /*
     * Rotates the given vertices in x and y by theta
     */
    static void rotate_vertices(const float* vertices,
                                float* out,
                                const unsigned int& vertex_count,
                                const unsigned int& component_count,
                                const float& theta) {
        const float c = cosf(theta);
        const float s = sinf(theta);
        for (unsigned int v = 0; v < vertex_count; ++v) {
            const float x = vertices[v * component_count + 0];
            const float y = vertices[v * component_count + 1];
            const float xx = x * c - y * s;
            const float yy = x * s + y * c;
            out[v * component_count + 0] = xx;
            out[v * component_count + 1] = yy;
            for (unsigned int c = 2; c < component_count; ++c) {
                out[v * component_count + c] = vertices[v * component_count + c];
            }
        }
    }

    static VertexArrayObject gen_circle_vao(
        const float& center_x,
        const float& center_y,
        const float& radius,
        const unsigned int& corners
    ) {
        const float z = 0.0f;
        //const float scale_x = 3.0f / 4.0f; // Scale in x with the aspect ratio, because the coords are between -1 and 1 in both directions
        const float scale_x = 1.0f;
        const unsigned int vertex_count = corners + 1; // One for the center aswell
        float vertices[3 * vertex_count];
        vertices[0] = center_x;
        vertices[1] = center_y;
        vertices[2] = z;
        for (unsigned int i = 0; i < corners; ++i) {
            const unsigned int index_base = 3 * (i + 1);
            const float angle = 2.0f * 3.14159265f * i / corners;
            const float x = center_x + radius * cosf(angle);
            const float y = center_y + radius * sinf(angle);
            vertices[index_base + 0] = x * scale_x;
            vertices[index_base + 1] = y;
            vertices[index_base + 2] = z;
        }

        const unsigned int index_count = 3 * corners;
        unsigned int indices[index_count];
        for (unsigned int i = 0; i < corners; ++i) {
            indices[3 * i + 0] = 0;
            indices[3 * i + 1] = 1 + i;
            indices[3 * i + 2] = 1 + (i + 1) % corners;
        }

        VertexArrayObject vao;
        vao.indexBuffer(indices, index_count)
                ->vertexBuffer(vertices, vertex_count, 3, 0);
        return vao;
    }

    static VertexArrayObject gen_spiral_vao(
        const float& center_x,
        const float& center_y,
        const float& radius,
        const unsigned int& corners,
        const unsigned int& loops,
        const float& stroke_width,
        const float& space
    ) {
        const float z = 0.0f;
        const float scale_x = 3.0f / 4.0f; // Scale in x with the aspect ratio, because the coords are between -1 and 1 in both directions
        const float decay = stroke_width + space; // how much it spirals inwards per turn
        const unsigned int vertices_per_loop = 2 * corners; // Inside and outside
        const unsigned int vertex_count = vertices_per_loop * loops;
        float vertices[3 * vertex_count];
        for (unsigned int i = 0; i < loops; ++i) {
            const float loop_decay = i * decay;
            for (unsigned int j = 0; j < corners; ++j) {
                const int index_base = 3 * ((i * vertices_per_loop) + (2 * j));
                const float corner_decay = j * decay / corners;
                const float angle = 2.0f * 3.14159265f * j / corners;
                const float inner_x = center_x + (radius - loop_decay - corner_decay - stroke_width) * cosf(angle);
                const float inner_y = center_y + (radius - loop_decay - corner_decay - stroke_width) * sinf(angle);
                const float outer_x = center_x + (radius - loop_decay - corner_decay) * cosf(angle);
                const float outer_y = center_y + (radius - loop_decay - corner_decay) * sinf(angle);
                vertices[index_base + 0] = inner_x * scale_x;
                vertices[index_base + 1] = inner_y;
                vertices[index_base + 2] = z;
                vertices[index_base + 3] = outer_x * scale_x;
                vertices[index_base + 4] = outer_y;
                vertices[index_base + 5] = z;
            }
        }

        const int index_count = 3 * corners * 2 * loops;
        unsigned int indices[index_count];
        unsigned int index = 0;
        for (unsigned int i = 0; i < loops; ++i) {
            unsigned const int end = i == loops - 1 ? corners - 1 : corners; // Skip the last edge due to overflow
            for (unsigned int j = 0; j < end; ++j) {
                const int inner_vertex = (i * vertices_per_loop) + (2 * j);
                const int outer_vertex = (i * vertices_per_loop) + (2 * j) + 1;
                const int next_inner_vertex = (i * vertices_per_loop) + (2 * ((j + 1)));
                const int next_outer_vertex = ((i * vertices_per_loop) + (2 * ((j + 1)))) + 1;
                indices[index++] = inner_vertex;
                indices[index++] = outer_vertex;
                indices[index++] = next_inner_vertex;
                indices[index++] = outer_vertex;
                indices[index++] = next_inner_vertex;
                indices[index++] = next_outer_vertex;
            }
        }

        VertexArrayObject vao;
        vao.indexBuffer(indices, index_count)
                ->vertexBuffer(vertices, vertex_count, 3, 0);
        return vao;
    }
}
