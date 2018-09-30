#pragma once

#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scenegraph {
    class SceneNode {
    public:

        void init(std::string type) {
            nodeType = type;
            position = glm::vec3(0, 0, 0);
            rotation = glm::vec3(0, 0, 0);
        }

        void init(std::string type, VertexArrayObject vao) {
            nodeType = type;
            this->vao = vao;
            position = glm::vec3(0, 0, 0);
            rotation = glm::vec3(0, 0, 0);
        }

        void addChild(SceneNode* child) {
            childs.push_back(child);
        }

        /*
         * Function that prints children nodes to stdout
         */
        void printChildren() {
            for (auto c : childs) {
                c->printChildren();
                std::cout << c->nodeType << "\n";
            }
        }

    protected:
        int vaoId; // Id of corresponding vertex array object
        std::vector<SceneNode*> childs; // Vector containing the scenenodes childs
        std::string nodeType;
        VertexArrayObject vao;

        glm::vec3 position; // Position of scenenode relative to its parent. x, y, z
        glm::vec3 rotation; // Rotation of scenenode relative to its parent. x, y, z
        glm::mat4 transformMatrix;
    };

    class SceneNodeFactory {
    public:
        SceneNode* createSceneNode(std::string type) {
            SceneNode* node = new SceneNode;
            node->init(type);
            return node;
        }

        SceneNode* createSceneNode(std::string type, VertexArrayObject vao) {
            SceneNode* node = new SceneNode;
            node->init(type, vao);
            return node;
        }

    };
}