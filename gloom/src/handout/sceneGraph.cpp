#include <memory>

#include "sceneGraph.hpp"
#include <iostream>

// --- Matrix Stack related functions ---

void SceneNode::addChild(std::shared_ptr<SceneNode> child) {
    children.push_back(child);
}

void SceneNode::visit(const glm::mat4 &parent_transformation,
                      const GLint& matrix_location) {
    currentTransformationMatrix = glm::translate(position)
            * glm::translate(referencePoint)
            * glm::rotate(rotation.z, glm::vec3(0,0,1))
            * glm::rotate(rotation.y, glm::vec3(0,1,0))
            * glm::rotate(rotation.x, glm::vec3(1,0,0))
            * glm::translate(-referencePoint);

    glm::mat4 mvp_matrix = parent_transformation * currentTransformationMatrix;

    glUniformMatrix4fv(
            matrix_location, // location
            1, // count: 1 matrix
            GL_FALSE, // do not transpose
            glm::value_ptr(mvp_matrix) // pointer to the matrix data
    );

    vao.render(); // only renders valid a valid vao

    for (std::shared_ptr<SceneNode> child : children) {
        child->visit(mvp_matrix, matrix_location);
    }
}

// You can use these to create a more "realistic" scene graph implementation

// Allocate a new empty matrix stack on the heap
std::stack<glm::mat4>* createEmptyMatrixStack() {
	return new std::stack<glm::mat4>();
}

// Push a matrix on top of the stack
void pushMatrix(std::stack<glm::mat4>* stack, glm::mat4 matrix) {
	stack->push(matrix);
}

// Remove a matrix from the top of the stack. The popped value is not returned.
void popMatrix(std::stack<glm::mat4>* stack) {
	stack->pop();
}

// Return the matrix which is currently at the top of the stack
glm::mat4 peekMatrix(std::stack<glm::mat4>* stack) {
	return stack->top();
}

// Pretty prints the values of a matrix to stdout. 
void printMatrix(glm::mat4 matrix) {
	float* values = glm::value_ptr(matrix);

	printf("(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n",
		values[0], values[4], values[8], values[12], 
		values[1], values[5], values[9], values[13], 
		values[2], values[6], values[10], values[14], 
		values[3], values[7], values[11], values[15]);
}

// --- Scene Graph related functions ---

// Creates an empty SceneNode instance.
// Values are initialised because otherwise they may contain garbage memory.
std::shared_ptr<SceneNode> createSceneNode() {
	return std::make_shared<SceneNode>();
}

// Pretty prints the current values of a SceneNode instance to stdout
void printNode(const std::shared_ptr<SceneNode>& node) {
	printf(
		"SceneNode {\n"
		"    Child count: %i\n"
		"    Rotation: (%f, %f, %f)\n"
		"    Location: (%f, %f, %f)\n"
		"    Reference point: (%f, %f, %f)\n"
		"}\n",
		int(node->children.size()),
		node->rotation.x, node->rotation.y, node->rotation.z,
		node->position.x, node->position.y, node->position.z,
		node->referencePoint.x, node->referencePoint.y, node->referencePoint.z);
}
