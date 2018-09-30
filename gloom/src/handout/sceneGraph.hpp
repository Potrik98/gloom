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
		position = float3(0, 0, 0);
		rotation = float3(0, 0, 0);

        referencePoint = float3(0, 0, 0);
	}

	// A list of all children that belong to this node.
	// For instance, in case of the scene graph of a human body shown in the assignment text, the "Upper Torso" node would contain the "Left Arm", "Right Arm", "Head" and "Lower Torso" nodes in its list of children.
	std::vector<SceneNode*> children;

	// The node's position and rotation relative to its parent
	float3 position;
	float3 rotation;

	// A transformation matrix representing the transformation of the node's location relative to its parent. This matrix is updated every frame.
	glm::mat4 currentTransformationMatrix;

	// The location of the node's reference point
	float3 referencePoint;

	// The ID of the VAO containing the "appearance" of this SceneNode.
	VertexArrayObject vao;
	void visit(const glm::mat4& parent_transformation,
	           const GLint& matrix_location);
	void addChild(SceneNode* child);
};

SceneNode* createSceneNode();
void printNode(SceneNode* node);

// For more details, see SceneGraph.cpp.
