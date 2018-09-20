#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::mat4 getViewMatrix() {
        glm::mat4 res;
        res = glm::rotate(res, -m_rotation.x, glm::vec3(1,0,0)); // rotate x
        res = glm::rotate(res, -m_rotation.y, glm::vec3(0,1,0)); // rotate y
        res = glm::rotate(res, -m_rotation.z, glm::vec3(0,0,1)); // rotate z
        res = glm::translate(res, -m_position);
        return res;
    }
private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
};
