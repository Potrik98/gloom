#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"

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
            // Create a vao with a color attribute
            float vertices[] = {
                0.4f, -0.2f, 0.0f,
                0.0f, 0.6f, 0.0f,
                -0.4f, -0.2f, 0.0f
            };

            float colors[] = {
                0.1f, 0.8f, 0.4f, 1.0f,
                1.0f, 0.4f, 0.2f, 1.0f,
                0.6f, 0.1f, 1.0f, 1.0f
            };

            int indices[] = {0, 1, 2};

            const int vertex_count = 3;
            const int index_count = 3;

            // Create a vao
            m_vao = VertexArrayObject(vertex_count);
            m_vao.indexArray(indices, index_count) 
                ->vertexArray(vertices, 3, 0) // Vertices in location 0
                ->vertexArray(colors, 4, 1); // Colors in location 1
        }
    };
}
