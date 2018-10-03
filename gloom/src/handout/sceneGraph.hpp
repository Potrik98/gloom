#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <stack>
#include <vector>
#include <cstdio>
#include <stdbool.h>
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include <fstream>
#include <memory>
#include "floats.hpp"

#include "vao.hpp"

// Matrix stack related functions
std::stack<glm::mat4>* createEmptyMatrixStack();
void pushMatrix(std::stack<glm::mat4>* stack, glm::mat4 matrix);
void popMatrix(std::stack<glm::mat4>* stack);
glm::mat4 peekMatrix(std::stack<glm::mat4>* stack);

void printMatrix(glm::mat4 matrix);

class SceneNode {
public:
	SceneNode() {
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);

        referencePoint = glm::vec3(0, 0, 0);
	}

	// A list of all children that belong to this node.
	// For instance, in case of the scene graph of a human body shown in the assignment text, the "Upper Torso" node would contain the "Left Arm", "Right Arm", "Head" and "Lower Torso" nodes in its list of children.
	std::vector<std::shared_ptr<SceneNode>> children;

	// The node's position and rotation relative to its parent
	glm::vec3 position;
	glm::vec3 rotation;

	// A transformation matrix representing the transformation of the node's location relative to its parent. This matrix is updated every frame.
	glm::mat4 currentTransformationMatrix;

	// The location of the node's reference point
	glm::vec3 referencePoint;

	// The ID of the VAO containing the "appearance" of this SceneNode.
	VertexArrayObject vao;

	/**
	 * Visit (update and render) the scene node,
	 * and all children nodes
	 * @param parent_mvp_matrix The mvp matrix of the parent scene node
	 * @param mvp_matrix_location The shader uniform location of the mvp matrix
	 */
	void visit(const glm::mat4& parent_mvp_matrix,
	           const GLint& mvp_matrix_location);

	/**
	 * Add a SceneNode to the children of this node.
	 *
	 * When adding a child, make a copy of the shared_ptr,
	 * so that the object still exists, even if the original
	 * shared_ptr is destroyed.
	 */
	void addChild(std::shared_ptr<SceneNode> child);
};

std::shared_ptr<SceneNode> createSceneNode();
void printNode(const std::shared_ptr<SceneNode>& node);

// For more details, see SceneGraph.cpp.
