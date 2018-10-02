#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"
#include "camera.hpp"
#include "handout/OBJLoader.hpp"
#include "handout/toolbox.hpp"
#include "handout/sceneGraph.hpp"

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

            m_camera = Camera(7.0f, 3.0f);
        }

        void render() override {
            const float delta_time = getTimeDeltaSeconds();
            m_camera.update(delta_time);

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
        GLint m_location_matrix;
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

            m_root = init_scene_graph();
        }

        void render() override {
            const float delta_time = getTimeDeltaSeconds();
            m_camera.update(delta_time);

            glm::mat4 transformation = m_projection_matrix * m_camera.getViewMatrix();
            m_shader.activate();
            m_root->visit(transformation, m_location_matrix);

            time += delta_time;
            float rotation_speed = 1.9f;
            float rotation_range = 0.8;
            arm_left_node->rotation.x = rotation_range * sinf(rotation_speed * time);
            arm_right_node->rotation.x = -rotation_range * sinf(rotation_speed * time);
            leg_left_node->rotation.x = -rotation_range * sinf(rotation_speed * time);
            leg_right_node->rotation.x = rotation_range * sinf(rotation_speed * time);
        }

    protected:
        SceneNode* m_root;
        SceneNode* arm_left_node;
        SceneNode* arm_right_node;
        SceneNode* leg_left_node;
        SceneNode* leg_right_node;
        float time = 0.0f;

        VertexArrayObject create_terrain() {
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
            return vaoFromMesh(generateChessboard(11, 9, 10.0f, color1, color2));
        }

        SceneNode* init_scene_graph() {
            VertexArrayObject terrain = create_terrain();

            SceneNode* root = createSceneNode();
            SceneNode* terrain_root = createSceneNode();
            terrain_root->vao = terrain;
            root->addChild(terrain_root);

            SceneNode* body = createSceneNode();
            body->vao = m_body;
            body->referencePoint = glm::vec3(0, 0, 0);
            root->addChild(body);

            arm_left_node = createSceneNode();
            arm_left_node->vao = m_arm_left;
            arm_left_node->referencePoint = glm::vec3(-6, 22, 0);
            body->addChild(arm_left_node);

            arm_right_node = createSceneNode();
            arm_right_node->vao = m_arm_right;
            arm_right_node->referencePoint = glm::vec3(6, 22, 0);
            body->addChild(arm_right_node);

            leg_left_node = createSceneNode();
            leg_left_node->vao = m_leg_left;
            leg_left_node->referencePoint = glm::vec3(6, 12, 0);

            body->addChild(leg_left_node);

            leg_right_node = createSceneNode();
            leg_right_node->vao = m_leg_right;
            leg_right_node->referencePoint = glm::vec3(2, 12, 0);
            body->addChild(leg_right_node);

            SceneNode* head = createSceneNode();
            head->vao = m_head;
            head->referencePoint = glm::vec3(0, 24, 0);
            body->addChild(head);

            return root;
        }
    };
}
