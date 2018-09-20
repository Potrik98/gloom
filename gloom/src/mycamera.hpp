#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyCamera {
    public:
        void init() {
            std::cout << "Initializing camera\n";
            position = glm::vec3(0, 0, 2);
        };

        glm::mat4x4 getCameraMatrix() {
            glm::mat4x4 camera;
            camera = glm::rotate(camera, rotation.x, glm::vec3(1, 0, 0)); // x-axis
            camera = glm::rotate(camera, rotation.y, glm::vec3(0, 1, 0)); // y-axis
            camera = glm::rotate(camera, rotation.z, glm::vec3(0, 0, 1)); // z-axis

            camera = glm::translate(camera, position); // Translate

            return camera;
        };

        void moveUp() {
            position.y += 0.05;
        }

        void moveDown() {
            position.y -= 0.05;
        }

        void moveRight() {
            position.x += 0.05;
        }

        void moveLeft() {
            position.x -= 0.05;
        }

        void rotatePositiveX() {
            rotation.x += 0.05;
        }

        void rotateNegativeX() {
            rotation.y -= 0.05;
        }

        void rotatePositiveY() {
            rotation.y += 0.05;
        }

        void rotateNegativeY() {
            rotation.y -= 0.05;
        }

    private:
        glm::vec3 position;
        glm::vec3 rotation;
};