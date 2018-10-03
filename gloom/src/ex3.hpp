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

    class Task345 : public Task2 {
    public:
        void init() override {
            Task2::init();

            m_root = init_scene_graph();

            // Set the camera orientation to something useful
            m_camera.set_position(glm::vec3(110.0f, 30.0f, 110.0f));
            m_camera.set_rotation(glm::vec3(-0.3f, 0.8f, 0.0f));
        }

        void render() override {
            const float delta_time = getTimeDeltaSeconds();
            m_camera.update(delta_time);

            glm::mat4 transformation = m_projection_matrix * m_camera.getViewMatrix();
            m_shader.activate();
            m_root->visit(transformation, m_location_matrix);

            animate(delta_time);
            move_path(delta_time);
        }

    protected:
        std::shared_ptr<SceneNode> m_root;
        std::shared_ptr<SceneNode> m_node_arm_left;
        std::shared_ptr<SceneNode> m_node_arm_right;
        std::shared_ptr<SceneNode> m_node_leg_left;
        std::shared_ptr<SceneNode> m_node_leg_right;
        std::shared_ptr<SceneNode> m_node_body;
        Path m_path = Path("../gloom/pathFiles/coordinates_0.txt");

        float m_time = 0.0f;
        const float TILE_SIZE = 10.0f;

        void animate(const float& delta_time) {
            m_time += delta_time;
            const float rotation_speed = 1.9f;
            const float rotation_range = 0.8;
            m_node_arm_left->rotation.x = rotation_range * sinf(rotation_speed * m_time);
            m_node_arm_right->rotation.x = -rotation_range * sinf(rotation_speed * m_time);
            m_node_leg_left->rotation.x = -rotation_range * sinf(rotation_speed * m_time);
            m_node_leg_right->rotation.x = rotation_range * sinf(rotation_speed * m_time);
        }

        void move_path(const float& delta_time) {
            glm::vec2 target = m_path.getCurrentWaypoint(TILE_SIZE);
            glm::vec2 d = glm::normalize(target - glm::vec2(m_node_body->position.x, m_node_body->position.z));
            const float movement_speed = 4.0f;
            glm::vec2 move = delta_time * movement_speed * d;
            m_node_body->position.x += move.x;
            m_node_body->position.z += move.y;

            if (m_path.hasWaypointBeenReached(glm::vec2(m_node_body->position.x, m_node_body->position.z), TILE_SIZE)) {
                m_path.advanceToNextWaypoint();
            }

            // Set body rotation to face the way we're moving
            m_node_body->rotation.y = atan2(d.x, d.y);
        }

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
            return vaoFromMesh(generateChessboard(9, 9, TILE_SIZE, color1, color2));
        }

        std::shared_ptr<SceneNode> init_scene_graph() {
            VertexArrayObject terrain = create_terrain();

            std::shared_ptr<SceneNode> root = createSceneNode();
            std::shared_ptr<SceneNode> terrain_root = createSceneNode();
            terrain_root->vao = terrain;
            root->addChild(terrain_root);

            m_node_body = createSceneNode();
            m_node_body->vao = m_body;
            m_node_body->referencePoint = glm::vec3(0, 0, 0);
            root->addChild(m_node_body);

            m_node_arm_left = createSceneNode();
            m_node_arm_left->vao = m_arm_left;
            m_node_arm_left->referencePoint = glm::vec3(-6, 22, 0);
            m_node_body->addChild(m_node_arm_left);

            m_node_arm_right = createSceneNode();
            m_node_arm_right->vao = m_arm_right;
            m_node_arm_right->referencePoint = glm::vec3(6, 22, 0);
            m_node_body->addChild(m_node_arm_right);

            m_node_leg_left = createSceneNode();
            m_node_leg_left->vao = m_leg_left;
            m_node_leg_left->referencePoint = glm::vec3(6, 12, 0);

            m_node_body->addChild(m_node_leg_left);

            m_node_leg_right = createSceneNode();
            m_node_leg_right->vao = m_leg_right;
            m_node_leg_right->referencePoint = glm::vec3(2, 12, 0);
            m_node_body->addChild(m_node_leg_right);

            std::shared_ptr<SceneNode> head = createSceneNode();
            head->vao = m_head;
            head->referencePoint = glm::vec3(0, 24, 0);
            m_node_body->addChild(head);

            return root;
        }
    };
}
