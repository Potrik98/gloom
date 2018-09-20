#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"
#include "mycamera.hpp"

#include <iostream>
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
            const unsigned int component_count = 4;
            const unsigned int value_count = m_vao.getVertexCount() * component_count;
            float colors[value_count];
            for (unsigned int i = 0; i < m_vao.getVertexCount(); ++i) {
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

    class Task3 : public Task1 {
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

            glUniform1f(m_location_a, a);
            glUniform1f(m_location_b, b);
            glUniform1f(m_location_c, c);
            glUniform1f(m_location_d, d);
            glUniform1f(m_location_e, e);
            glUniform1f(m_location_f, f);

            BaseTask::render();

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


    class Task4 : public Task1 {
    public:
        void init() {
            // Create the shader program
            m_shader.makeBasicShader("../gloom/shaders/matrix.vert",
                                     "../gloom/shaders/simple.frag");
            m_location_matrix = glGetUniformLocation(m_shader.program_id(), "matrix");
            /*float m_matrix_vals[] = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f,  0.0f, 0.0f, 1.0f,
            };*/

            m_matrix = glm::mat4(1.0f);

            m_perspective_matrix = glm::perspective(
                0.87266f, 
                1.33f, 
                1.0f, 
                100.0f);

            init_vao();
        }

        void render() {

            glUniformMatrix4fv(m_location_matrix, 1, GL_FALSE, glm::value_ptr(m_perspective_matrix));

            BaseTask::render();
        }
    protected:
        int m_location_matrix;
        glm::mat4 m_matrix;
        glm::mat4x4 m_perspective_matrix;
    };


    class Task4c : public Task1 {
    public:
        void init() {
            // Create the shader program
            m_shader.makeBasicShader("../gloom/shaders/matrix.vert",
                                     "../gloom/shaders/simple.frag");
            m_location_matrix = glGetUniformLocation(m_shader.program_id(), "matrix");
            camera.init();

            m_matrix = glm::mat4x4(1.0f);
            
            m_perspective_matrix = glm::perspective(
                0.87266f, 
                1.33f, 
                1.0f, 
                100.0f);

            init_vao();
        }

        void render(GLFWwindow* window) {
            handleKeyboardInput(window); // Handling keyboard input

            glm::mat4x4 matrix = m_matrix * camera.getCameraMatrix();

            glUniformMatrix4fv(m_location_matrix, 1, GL_FALSE, glm::value_ptr(matrix));

            BaseTask::render();
        }

        void handleKeyboardInput(GLFWwindow* window)
        {
            // Use escape key for terminating the GLFW window
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                camera.moveUp();
            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                camera.moveDown();
            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                camera.moveRight();
            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                camera.moveLeft();
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera.rotatePositiveY();
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                camera.rotateNegativeX();
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera.rotateNegativeY();
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                camera.rotatePositiveX();
            }
        }
    protected:
        int m_location_matrix;
        glm::mat4 m_matrix;
        glm::mat4x4 m_perspective_matrix;
        glm::vec3 m_pos;
        glm::vec3 m_rotation;
        MyCamera camera;
    };




}
