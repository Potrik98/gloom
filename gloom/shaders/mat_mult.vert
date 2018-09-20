#version 430 core

layout (location = 0) in vec3 position;

void main()
{
    vec4 new_position = vec4(position.x, position.y, position.z, 1);
    mat4 matrix = mat4(
        0.5, 1, 0, 0,
        0, 0.5, 0, 0,
        0, 0, 0.5, 0,
        0, 0, 0, 1
    );
    gl_Position = new_position * matrix;
}
