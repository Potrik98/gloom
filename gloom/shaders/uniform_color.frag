#version 430 core

out vec4 color;

uniform float r;
uniform float g;
uniform float b;

void main()
{
    color = vec4(r, g, b, 1.0);
}
