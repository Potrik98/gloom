#version 430 core

out vec4 color;

void main()
{
    int grid_size = 32;
    if ((((int(gl_FragCoord.x / grid_size) + int(gl_FragCoord.y / grid_size))) % 2) == 1) {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
