#version 430 core

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0f);

    vec3 new_position = vec3(-position.xy, position.z);
    //gl_Position = vec4(new_position, 1.0f); // task 2d
}
