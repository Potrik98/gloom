#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
            const unsigned int color_component_count = 4;
            const unsigned int value_count = m_vao.getVertexCount() * color_component_count;
            float colors[value_count];
            // Generate some colors using some formulas
            for (unsigned int i = 0; i < m_vao.getVertexCount(); ++i) {
                float r = 0.5f + 0.5f * cosf(i * 2.1f);
                float g = 0.5f + 0.5f * cosf(i * 1.7f + 0.2f);
                float b = 0.5f + 0.5f * cosf(i * 0.5f + 2.7f);
                float a = 1.0f;
                colors[color_component_count * i + 0] = r;
                colors[color_component_count * i + 1] = g;
                colors[color_component_count * i + 2] = b;
                colors[color_component_count * i + 3] = a;
            }
            m_vao.vertexArray(colors, color_component_count, 1); // Add colors to location 1
        }
    };

    class Task2 : public AdvancedTask { // Advanced tasks contain multiple vaos
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
            // The vertices of one triangle
            float triangle_vertices[] = {
                0.5f, 0.0f, 0.0f,
                -0.5f, 0.3f, 0.0f,
                -0.5f, -0.3f, 0.0f
            };
            int indices[] = {0, 1, 2};

            // The colors of each triangle
            float triangle_colors[] = {
                1.0f, 0.5f, 0.0f, 0.7f,
                0.5f, 0.1f, 1.0f, 0.7f,
                0.2f, 0.7f, 0.7f, 0.7f
            };

            // The z value of each triangle
            float triangle_z[] = {
                0.9f,
                0.8f,
                0.7f
            };

            const unsigned int vertex_count = 3;
            const unsigned int index_count = 3;
            const unsigned int component_count = 3;
            const unsigned int color_components = 4;

            // Add 3 rotated triangles
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
                ); // Rotate by angle theta

                // Update the z value and color of each triangle
                float colors[vertex_count * color_components];
                for (unsigned int v = 0; v < vertex_count; ++v) {
                    vertices[v * component_count + 2] = triangle_z[i];
                    for (unsigned int c = 0; c < color_components; ++c) {
                        colors[v * color_components + c] = triangle_colors[i * color_components + c];
                    }
                }

                // Add the vao to the list
                VertexArrayObject vao(vertex_count);
                vao.indexArray(indices, index_count)
                  ->vertexArray(vertices, 3, 0)
                  ->vertexArray(colors, 4, 1);
                
                m_vaos.push_back(vao);
            }
        }
    };

    class Task3 : public Task1 { // Extend task 1 to use the vao
    public:
        void init() {
            // Create the shader program
            m_shader.makeBasicShader("../gloom/shaders/matrix.vert",
                                     "../gloom/shaders/color.frag");

            m_location_a = glGetUniformLocation(m_shader.program_id(), "a");
            m_location_b = glGetUniformLocation(m_shader.program_id(), "b");
            m_location_c = glGetUniformLocation(m_shader.program_id(), "c");
            m_location_d = glGetUniformLocation(m_shader.program_id(), "d");
            m_location_e = glGetUniformLocation(m_shader.program_id(), "e");
            m_location_f = glGetUniformLocation(m_shader.program_id(), "f");

            
            m_step = 0.0f;
            init_vao();
        }

        void render() {
            float a = cosf(m_step);
            float b = -sinf(m_step);
            float c = 0.0f;
            float d = sinf(m_step);
            float e = cosf(m_step);
            float f = 0.0f;

            m_shader.activate();

            glUniform1f(m_location_a, a);
            glUniform1f(m_location_b, b);
            glUniform1f(m_location_c, c);
            glUniform1f(m_location_d, d);
            glUniform1f(m_location_e, e);
            glUniform1f(m_location_f, f);

            m_vao.render();

            m_step += 0.0005f;
        }
    protected:
        int m_location_a;
        int m_location_b;
        int m_location_c;
        int m_location_d;
        int m_location_e;
        int m_location_f;
        float m_step;
    };

    class Task4 : public Task1 { // Extend task 1 to use the vao
    public:
        void init() {
            // Create the shader program
            m_shader.makeBasicShader("../gloom/shaders/uniform_matrix.vert",
                                     "../gloom/shaders/simple.frag");

            m_location_matrix = glGetUniformLocation(m_shader.program_id(), "matrix");
            init_vao();

            m_matrix = glm::perspective(
                0.87266f, // FOV Y = 50 degrees
                ((float) windowWidth) / windowHeight, // Aspect ratio
                1.0f, // Near plane
                100.0f // Far plane
            );

            // Translate the object so it is visible
            glm::mat4 translation;
            translation = glm::translate(translation, glm::vec3(0, 0, -2.0f));

            m_matrix = m_matrix * translation;
        }

        void render() {
            m_shader.activate();
            glUniformMatrix4fv(
                m_location_matrix, // location
                1, // count: 1 matrix
                GL_FALSE, // do not transpose
                glm::value_ptr(m_matrix) // pointer to the matrix data
            );
            m_vao.render();
        }
    protected:
        int m_location_matrix;
        glm::mat4 m_matrix;
    };
}
