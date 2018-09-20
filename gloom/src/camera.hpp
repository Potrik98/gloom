#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.hpp"

class Camera {
public:
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

    void update() {
        // Check keyboard input and update orientation
        if (Input::getInstance().getKeyState(GLFW_KEY_A)) m_position.x -= m_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_D)) m_position.x += m_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_W)) m_position.z -= m_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_S)) m_position.z += m_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_C)) m_position.y -= m_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_R)) m_position.y += m_speed;
        
        if (Input::getInstance().getKeyState(GLFW_KEY_LEFT)) m_rotation.y -= m_rotation_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_RIGHT)) m_rotation.y += m_rotation_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_UP)) m_rotation.x += m_rotation_speed;
        if (Input::getInstance().getKeyState(GLFW_KEY_DOWN)) m_rotation.x -= m_rotation_speed;
    }
private:
    glm::vec3 m_position = glm::vec3(0, 0, 2);
    glm::vec3 m_rotation;
    float m_speed = 0.001f;
    float m_rotation_speed = 0.0007f;
};
