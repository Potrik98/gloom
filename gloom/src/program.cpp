// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "ex1.hpp"
#include "ex2.hpp"

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    /*
    // uniforms for task 3d
    const int location_r = glGetUniformLocation(shader.program_id(), "r");
    const int location_g = glGetUniformLocation(shader.program_id(), "g");
    const int location_b = glGetUniformLocation(shader.program_id(), "b");

    int time = 0;
    */

    ex1::Task2d task;
    task.init();

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        task.render();

        /*
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
        */

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
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
