#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

out vec4 _color;
out vec3 _normal;

uniform mat4 matrix;
uniform mat4 model_matrix = mat4(1.0);

void main()
{
    _color = color;
    _normal = normal;
    gl_Position = matrix * model_matrix * position;
}
