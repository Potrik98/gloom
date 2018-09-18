#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertexColorIn;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    vertexColor = vertexColorIn;
}
