#include <memory>

#include "sceneGraph.hpp"
#include <iostream>

// --- Matrix Stack related functions ---

void SceneNode::addChild(std::shared_ptr<SceneNode> child) {
    children.push_back(child);
}

void SceneNode::visit(const glm::mat4 &parent_mvp_matrix,
                      const GLint& mvp_matrix_location) {
    currentTransformationMatrix = glm::translate(position)
            * glm::translate(referencePoint)
            * glm::rotate(rotation.z, glm::vec3(0,0,1))
            * glm::rotate(rotation.y, glm::vec3(0,1,0))
            * glm::rotate(rotation.x, glm::vec3(1,0,0))
            * glm::translate(-referencePoint);

    // We skip calculating the model matrix of this node,
    // because we can move straight to the mvp matrix by
    // applying the current node transformation.
    // We could alternatively make the visit function take in
    // the vp matrix as an argument, i.e.
    // visit(parent_model_matrix, vp_matrix, mvp_matrix_location)
    // and then the current model matrix would be
    // ml_matrix = parent_ml_matrix * currentTransformationMatrix
    // and the mvp matrix would be
    // mvp_matrix = vp_matrix * ml_matrix
    // but this approach seems inelegant, and is also
    // more computationally expensive, requiring two matrix
    // multiplications per visited node instead of one.
    glm::mat4 mvp_matrix = parent_mvp_matrix * currentTransformationMatrix;

    glUniformMatrix4fv(
            mvp_matrix_location, // location
            1, // count: 1 matrix
            GL_FALSE, // do not transpose
            glm::value_ptr(mvp_matrix) // pointer to the matrix data
    );

    vao.render(); // only renders valid vaos (because vaoID = 0 doesn't render)

    for (const std::shared_ptr<SceneNode> &child : children) {
        child->visit(mvp_matrix, mvp_matrix_location);
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
