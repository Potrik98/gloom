#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"

namespace ex1 {
    class Task1 : public BaseTask {
    protected:
        void init_vao() {
            // Generate rotated triangles
            float v[] = {
                0.2f, 0.2f, 0.0f,
                0.0f, 0.7f, 0.0f,
                -0.2f, 0.2f, 0.0f,
            };

            const int sides = 5;
            const int vertex_count = 3 * sides;
            float vertices[vertex_count * 3];

            int index = 0;
            for (int i = 0; i < sides; ++i) {
                float theta = (3.1415927f * 2.0f / sides) * i;
                float c = cosf(theta);
                float s = sinf(theta);

                for (int j = 0; j < 3; ++j) {
                    float x = v[3 * j + 0];
                    float y = v[3 * j + 1];
                    float xx = x * c - y * s;
                    float yy = x * s + y * c;
                    vertices[index++] = xx;
                    vertices[index++] = yy;
                    vertices[index++] = 0.0f;
                }
            }
            
            const unsigned int index_count = vertex_count;
            int indices[vertex_count];
            for (int i = 0; i < vertex_count; i++) {
                indices[i] = i;
            }
            m_vao = VertexArrayObject(vertex_count);
            m_vao.indexArray(indices, index_count)
                ->vertexArray(vertices, 3, 0);
        }
    };

    class Task2a : public BaseTask {
    protected:
        void init_vao() {
            float vertices[] = {
                0.6f, -0.8f, -1.2f,
                0.0f, 0.4f, 0.0f,
                -0.8f, -0.2f, 1.2f
            };

            int indices[] = {1, 2, 3};

            const int vertex_count = 3;
            const int index_count = 3;

            m_vao = VertexArrayObject(vertex_count);
            m_vao.indexArray(indices, index_count)
                ->vertexArray(vertices, 3, 0);
        }
    };

    class Task2d : public Task2a {
    public:
        void init() {
            m_shader.makeBasicShader("../gloom/shaders/flip.vert", "../gloom/shaders/blue.frag");
            init_vao();
        }
    };

    class Task3a : public Task2a {
    protected:
        public:
        void init() {
            m_shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/checkers.frag");
            init_vao();
        }
    };

    class Task3b : public BaseTask {
    protected:
        void init_vao() {
            m_vao = geometry::gen_circle_vao(
                0.0f, // center x
                0.0f, // center y
                0.5f, // radius
                32 // corners
            );
        }
    };

    class Task3c : public BaseTask {
    protected:
        void init_vao() {
            m_vao = geometry::gen_spiral_vao(
                0.0f, // center x
                0.0f, // center y
                0.5f, // radius
                32, // corners
                5, // loops
                0.05f, // stroke width
                0.05f // space
            );
        }
    };
}
