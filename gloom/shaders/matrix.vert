#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color_in;

out vec4 vertex_color;

uniform mat4 matrix;

void main()
{
    vertex_color = vertex_color_in;
    gl_Position = matrix * vec4(position, 1.0);
}
