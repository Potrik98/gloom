#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color_in;

out vec4 vertex_color;

uniform float a;
uniform float b;
uniform float c;
uniform float d;
uniform float e;
uniform float f;

void main()
{
    mat4 matrix = mat4(
          a,   b, 0.0,   c,
          d,   e, 0.0,   f,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    vertex_color = vertex_color_in;
    gl_Position = matrix * vec4(position, 1.0);
}
