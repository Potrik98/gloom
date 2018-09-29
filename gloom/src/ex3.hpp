#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"
#include "camera.hpp"
#include "handout/OBJLoader.hpp"
#include "handout/toolbox.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ex3 {
    class Task2 : public Task {
    public:
        void init() override {
            MinecraftCharacter character = loadMinecraftCharacterModel("../gloom/res/steve.obj");
            m_head = vaoFromMesh(character.head);
            m_body = vaoFromMesh(character.torso);
            m_arm_left = vaoFromMesh(character.leftArm);
            m_arm_right = vaoFromMesh(character.rightArm);
            m_leg_left = vaoFromMesh(character.leftLeg);
            m_leg_right = vaoFromMesh(character.rightLeg);

            // Create a basic shader program
            m_shader.makeBasicShader("../gloom/shaders/shader.vert",
                                     "../gloom/shaders/shader.frag");
            m_location_matrix = glGetUniformLocation(m_shader.program_id(), "matrix");

            m_projection_matrix = glm::perspective(
                    0.87266f, // FOV Y = 50 degrees
                    ((float) windowWidth) / windowHeight, // Aspect ratio
                    1.0f, // Near plane
                    500.0f // Far plane
            );

            m_camera.set_speed(0.01f);
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
            m_head.render();
            m_body.render();
            m_arm_left.render();
            m_arm_right.render();
            m_leg_left.render();
            m_leg_right.render();
        }
    protected:
        Gloom::Shader m_shader;
        GLuint m_location_matrix;
        VertexArrayObject m_head;
        VertexArrayObject m_body;
        VertexArrayObject m_arm_left;
        VertexArrayObject m_arm_right;
        VertexArrayObject m_leg_left;
        VertexArrayObject m_leg_right;
        glm::mat4 m_projection_matrix;
        Camera m_camera;
    };

    class Task3 : public Task2 {
    public:
        void init() override {
            Task2::init();
            float4 color1 = float4(
                    182/256.0f,
                    206/256.0f,
                    167/256.0f,
                    1.0f);
            float4 color2 = float4(
                    195/256.0f,
                    167/256.0f,
                    206/255.0f,
                    1.0f);
            m_terrain = vaoFromMesh(
                    generateChessboard(11, 9, 10.0f, color1, color2));
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
            m_head.render();
            m_body.render();
            m_arm_left.render();
            m_arm_right.render();
            m_leg_left.render();
            m_leg_right.render();
            m_terrain.render();
        }

    protected:
        VertexArrayObject m_terrain;
    };
}
