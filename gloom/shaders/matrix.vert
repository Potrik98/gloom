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
          a,   d, 0.0, 0.0,
          b,   e, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
          c,   f, 0.0, 1.0
    ); // OpenGL matrices are column major
    vertex_color = vertex_color_in;
    gl_Position = matrix * vec4(position, 1.0);
}
