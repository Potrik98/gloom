// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include <math.h>

int createVertexArrayObject(
        const float* vertices,
        const int* indices,
        const int vertex_count,
        const int component_count,
        const int index_count) {
    // Generate vao
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);

    glBindVertexArray(vao_id);

    // Vertex Buffer
    unsigned int buf_id;
    glGenBuffers(1, &buf_id);

    glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * component_count * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, // location = 0
        component_count, // vector components
        GL_FLOAT, // float data
        GL_FALSE,
        0,
        0
    );
    glEnableVertexAttribArray(0); // Enable location = 0

    // Index buffer
    unsigned int idx_buf_id;
    glGenBuffers(1, &idx_buf_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buf_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(int), indices, GL_STATIC_DRAW);

    return vao_id;
}

void get_task_1_vao(int& vao_id, int& index_count) {
    // Generate rotated triangles
    float v[] = {
        0.2f, 0.2f, 0.0f,
        0.0f, 0.7f, 0.0f,
        -0.2f, 0.2f, 0.0f,
    };

    const int sides = 5;
    const int vertex_count = 3 * sides;
    float vertices[vertex_count * 3];

    int index = 0;
    for (int i = 0; i < sides; ++i) {
        float theta = (3.1415927f * 2.0f / sides) * i;
        float c = cosf(theta);
        float s = sinf(theta);

        for (int j = 0; j < 3; ++j) {
            float x = v[3 * j + 0];
            float y = v[3 * j + 1];
            float xx = x * c - y * s;
            float yy = x * s + y * c;
            vertices[index++] = xx;
            vertices[index++] = yy;
            vertices[index++] = 0.0f;
        }
    }
    
    index_count = vertex_count;
    int indices[vertex_count];
    for (int i = 0; i < vertex_count; i++) {
        indices[i] = i;
    }

    // Create a vao
    vao_id = createVertexArrayObject(
        vertices,
        indices,
        vertex_count,
        3, // 3 components per vertex (xyz)
        index_count
    );
}

void get_task_2_vao(int& vao_id, int& index_count) {
    float vertices[] = {
        0.6f, -0.8f, -1.2f,
        0.0f, 0.4f, 0.0f,
        -0.8f, -0.2f, 1.2f
    };

    int indices[] = {1, 2, 3};

    const int vertex_count = 3;
    index_count = 3;

    // Create a vao
    vao_id = createVertexArrayObject(
        vertices,
        indices,
        vertex_count,
        3, // 3 components per vertex (xyz)
        index_count
    );
}

void gen_circle_vao(int& vao_id, int& index_count) {
    const float center_x = 0.0f;
    const float center_y = 0.0f;
    const float z = 0.0f;
    const float scale_x = 3.0f / 4.0f; // Scale in x with the aspect ratio, because the coords are between -1 and 1 in both directions
    const float radius = 0.5f;
    const int corners = 64;
    const int vertex_count = corners + 1; // One for the center aswell
    float vertices[3 * vertex_count];
    vertices[0] = center_x;
    vertices[1] = center_y;
    vertices[2] = z;
    for (int i = 0; i < corners; ++i) {
        const int index_base = 3 * (i + 1);
        const float angle = 2.0f * 3.14159265f * i / corners;
        const float x = center_x + radius * cosf(angle);
        const float y = center_y + radius * sinf(angle);
        vertices[index_base + 0] = x * scale_x;
        vertices[index_base + 1] = y;
        vertices[index_base + 2] = z;
    }

    index_count = 3 * corners;
    int indices[index_count];
    for (int i = 0; i < corners; ++i) {
        indices[3 * i + 0] = 0;
        indices[3 * i + 1] = 1 + i;
        indices[3 * i + 2] = 1 + (i + 1) % corners;
    }

    // Create a vao
    vao_id = createVertexArrayObject(
        vertices,
        indices,
        vertex_count,
        3, // 3 components per vertex (xyz)
        index_count
    );
}

void gen_spiral_vao(int& vao_id, int& index_count) {
    const float center_x = 0.0f;
    const float center_y = 0.0f;
    const float z = 0.0f;
    const float scale_x = 3.0f / 4.0f; // Scale in x with the aspect ratio, because the coords are between -1 and 1 in both directions
    const float radius = 0.6f;
    const int corners = 32;
    const int loops = 5;
    const float width = 0.05f;
    const float space = 0.05f;
    const float decay = width + space; // how much it spirals inwards per turn
    const int vertices_per_loop = 2 * corners; // Inside and outside
    const int vertex_count = vertices_per_loop * loops;
    float vertices[3 * vertex_count];
    for (int i = 0; i < loops; ++i) {
        const float loop_decay = i * decay;
        for (int j = 0; j < corners; ++j) {
            const int index_base = 3 * ((i * vertices_per_loop) + (2 * j));
            const float corner_decay = j * decay / corners;
            const float angle = 2.0f * 3.14159265f * j / corners;
            const float inner_x = center_x + (radius - loop_decay - corner_decay - width) * cosf(angle);
            const float inner_y = center_y + (radius - loop_decay - corner_decay - width) * sinf(angle);
            const float outer_x = center_x + (radius - loop_decay - corner_decay) * cosf(angle);
            const float outer_y = center_y + (radius - loop_decay - corner_decay) * sinf(angle);
            vertices[index_base + 0] = inner_x * scale_x;
            vertices[index_base + 1] = inner_y;
            vertices[index_base + 2] = z;
            vertices[index_base + 3] = outer_x * scale_x;
            vertices[index_base + 4] = outer_y;
            vertices[index_base + 5] = z;
        }
    }

    index_count = 3 * corners * 2 * loops;
    int indices[index_count];
    int index = 0;
    for (int i = 0; i < loops; ++i) {
        const int end = i == loops - 1 ? corners - 1 : corners; // Skip the last edge due to overflow
        for (int j = 0; j < end; ++j) {
            const int inner_vertex = (i * vertices_per_loop) + (2 * j);
            const int outer_vertex = (i * vertices_per_loop) + (2 * j) + 1;
            const int next_inner_vertex = (i * vertices_per_loop) + (2 * ((j + 1)));
            const int next_outer_vertex = ((i * vertices_per_loop) + (2 * ((j + 1)))) + 1;
            indices[index++] = inner_vertex;
            indices[index++] = outer_vertex;
            indices[index++] = next_inner_vertex;
            indices[index++] = outer_vertex;
            indices[index++] = next_inner_vertex;
            indices[index++] = next_outer_vertex;
        }
    }

    // Create a vao
    vao_id = createVertexArrayObject(
        vertices,
        indices,
        vertex_count,
        3, // 3 components per vertex (xyz)
        index_count
    );
}

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    //glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Create the shader program
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");

    int vao_id;
    int index_count;
    get_task_2_vao(vao_id, index_count);

    // uniforms for task 3d
    const int location_r = glGetUniformLocation(shader.program_id(), "r");
    const int location_g = glGetUniformLocation(shader.program_id(), "g");
    const int location_b = glGetUniformLocation(shader.program_id(), "b");

    int time = 0;

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        shader.activate();

        // uniforms for task 3d
        float r = 0.5f + 0.5f * cosf(time / 30.0f);
        float g = 0.5f + 0.5f * cosf(time / 100.0f + 0.3f);
        float b = 0.5f + 0.5f * cosf(time / 450.0f + 0.7f);

        glUniform1f(location_r, r);
        glUniform1f(location_g, g);
        glUniform1f(location_b, b);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);

        ++time;
    }
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
