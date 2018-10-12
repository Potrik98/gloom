#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.hpp"

class Camera {
public:
    Camera() {};

    Camera(const float& movement_speed,
           const float& rotation_speed) {
        m_speed = movement_speed;
        m_rotation_speed = rotation_speed;
    }

    glm::mat4 getViewMatrix() {
        glm::mat4 res;
        // Apply rotation in the three axises, and translate the camera
        // (Multiplies the transformation matrices into one view matrix)
        res = glm::rotate(res, -m_rotation.x, glm::vec3(1,0,0)); // rotate x
        res = glm::rotate(res, -m_rotation.y, glm::vec3(0,1,0)); // rotate y
        res = glm::rotate(res, -m_rotation.z, glm::vec3(0,0,1)); // rotate z
        res = glm::translate(res, -m_position);
        return res;
    }

    void update(const float& delta_time) {
        // Check keyboard input and update orientation
        float x_dir = 0.0f;
        float z_dir = 0.0f;
        if (Input::getInstance().getKeyState(GLFW_KEY_A)) x_dir -= m_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_D)) x_dir += m_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_W)) z_dir -= m_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_S)) z_dir += m_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_C)) m_position.y -= m_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_R)) m_position.y += m_speed * delta_time;
        m_position.x += cosf(-m_rotation.y) * x_dir - sinf(-m_rotation.y) * z_dir;
        m_position.z += sinf(-m_rotation.y) * x_dir + cosf(-m_rotation.y) * z_dir;

        if (Input::getInstance().getKeyState(GLFW_KEY_LEFT)) m_rotation.y += m_rotation_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_RIGHT)) m_rotation.y -= m_rotation_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_UP)) m_rotation.x += m_rotation_speed * delta_time;
        if (Input::getInstance().getKeyState(GLFW_KEY_DOWN)) m_rotation.x -= m_rotation_speed * delta_time;
    }

    void set_speed(const float& speed) {
        m_speed = speed;
    }

    void set_position(const glm::vec3& pos) {
        m_position = pos; // will copy the data
    }

    void set_rotation(const glm::vec3& rot) {
        m_rotation = rot;
    }
private:
    glm::vec3 m_position = glm::vec3(0, 0, 2);
    glm::vec3 m_rotation;
    float m_speed = 0.001f;
    float m_rotation_speed = 0.0007f;
};
