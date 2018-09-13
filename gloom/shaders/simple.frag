#version 430 core

out vec4 color;

uniform float r;
uniform float g;
uniform float b;

void main()
{
    // task 2d
    color = vec4(0.0f, 0.2f, 1.0f, 1.0f);
    
    // task 3a
    int grid_size = 32;
    if ((((int(gl_FragCoord.x / grid_size) + int(gl_FragCoord.y / grid_size))) % 2) == 1) {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }

    // task 3d
    color = vec4(r, g, b, 1.0);
    
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
