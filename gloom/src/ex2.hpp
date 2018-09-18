#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"

#include "math.h"

namespace ex2 {
    class Task1 : public BaseTask {
    public:
        void init() {
            // Create the shader program
            m_shader.makeBasicShader("../gloom/shaders/color.vert",
                                     "../gloom/shaders/color.frag");
            init_vao();
        }
    protected:
        void init_vao() {
            m_vao = geometry::gen_circle_vao(
                0.0f, // center x
                0.0f, // center y
                0.5f, // radius
                5 // corners
            );
            const unsigned int component_count = 4;
            const unsigned int value_count = m_vao.getVertexCount() * component_count;
            float colors[value_count];
            for (int i = 0; i < m_vao.getVertexCount(); ++i) {
                float r = 0.5f + 0.5f * cosf(i * 2.1f);
                float g = 0.5f + 0.5f * cosf(i * 1.7f + 0.2f);
                float b = 0.5f + 0.5f * cosf(i * 0.5f + 2.7f);
                float a = 1.0f;
                colors[component_count * i + 0] = r;
                colors[component_count * i + 1] = g;
                colors[component_count * i + 2] = b;
                colors[component_count * i + 3] = a;
            }
            m_vao.vertexArray(colors, component_count, 1); // Add colors to location 1
        }
    };

    class Task2 : public AdvancedTask {
    public:
        void init() {
            // Create a basic shader program
            m_shader.makeBasicShader("../gloom/shaders/color.vert",
                                     "../gloom/shaders/color.frag");
            // Initialize the vaos
            init_vaos();
        }
    protected:
        virtual void init_vaos() {
            float triangle_vertices[] = {
                0.5f, 0.0f, 0.0f,
                -0.5f, 0.3f, 0.0f,
                -0.5f, -0.3f, 0.0f
            };

            float triangle_colors[] = {
                1.0f, 0.5f, 0.0f, 0.7f,
                0.5f, 0.1f, 1.0f, 0.7f,
                0.2f, 0.7f, 0.7f, 0.7f
            };

            float triangle_z[] = {
                0.9f,
                0.8f,
                0.7f
            };

            int indices[] = {0, 1, 2};

            const unsigned int vertex_count = 3;
            const unsigned int index_count = 3;
            const unsigned int component_count = 3;
            const unsigned int color_components = 4;

            const unsigned int triangle_count = 3;
            for (unsigned int i = 0; i < triangle_count; ++i) {
                const float theta = 2.0f * 3.14159265f / triangle_count * i;
                float vertices[vertex_count * component_count];
                geometry::rotate_vertices(
                    triangle_vertices,
                    vertices,
                    vertex_count,
                    component_count,
                    theta
                );
                float colors[vertex_count * color_components];
                for (unsigned int v = 0; v < vertex_count; ++v) {
                    vertices[v * component_count + 2] = triangle_z[i];
                    for (unsigned int c = 0; c < color_components; ++c) {
                        colors[v * color_components + c] = triangle_colors[i * color_components + c];
                    }
                }
                VertexArrayObject vao(vertex_count);
                vao.indexArray(indices, index_count)
                  ->vertexArray(vertices, 3, 0)
                  ->vertexArray(colors, 4, 1);
                
                m_vaos.push_back(vao);
            }
        }
    };
}
