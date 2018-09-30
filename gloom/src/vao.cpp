#include "vao.hpp"

VertexArrayObject vaoFromMesh(const Mesh &mesh) {
    const unsigned int vertex_count = mesh.vertices.size();
    VertexArrayObject result;
    result.indexBuffer(mesh.indices)
            ->vertexBuffer(mesh.vertices, 0)
            ->vertexBuffer(mesh.colours, 1);
    if (mesh.hasNormals) {
        result.vertexBuffer(mesh.normals, 2);
    }
    return result;
}
