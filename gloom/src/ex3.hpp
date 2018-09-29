#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"
#include "camera.hpp"
#include "handout/OBJLoader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ex3 {
    class Task2 : public Task {
    public:
        void init() override {
            MinecraftCharacter character = loadMinecraftCharacterModel("../gloom/res/steve.obj");
            VertexArrayObject head = vaoFromMesh(character.head);
            m_vao = head;

            // Create a basic shader program
            m_shader.makeBasicShader("../gloom/shaders/shader.vert",
                                     "../gloom/shaders/shader.frag");
            m_location_matrix = glGetUniformLocation(m_shader.program_id(), "matrix");

            m_projection_matrix = glm::perspective(
                    0.87266f, // FOV Y = 50 degrees
                    ((float) windowWidth) / windowHeight, // Aspect ratio
                    1.0f, // Near plane
                    100.0f // Far plane
            );
        }

        void render() override {
            m_camera.update();

            glm::mat4 transformation =
                    m_projection_matrix * m_camera.getViewMatrix();
            m_shader.activate();
            glUniformMatrix4fv(
                    m_location_matrix, // location
                    1, // count: 1 matrix
                    GL_FALSE, // do not transpose
                    glm::value_ptr(transformation) // pointer to the matrix data
            );
            m_vao.render();
        }
    protected:
        Gloom::Shader m_shader;
        GLuint m_location_matrix;
        VertexArrayObject m_vao;
        glm::mat4 m_projection_matrix;
        Camera m_camera;
    };
}
