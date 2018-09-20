#pragma once

#include <GLFW/glfw3.h>

#define KEY_COUNT 256

/*
 * Singleton class for handling input and managing input state
 */
class Input {
public:
    static Input& getInstance(GLFWwindow* window = nullptr) {
        static Input instance(window);
        return instance;
    }

    bool getKeyState(const unsigned int& key) const {
        if (key < KEY_COUNT) return m_keys_pressed[key];
        return false;
    }
private:
    Input(GLFWwindow* window) {
        for (int i = 0; i < KEY_COUNT; ++i) m_keys_pressed[i] = false;
        // Set the window user to this input
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, [] (GLFWwindow* window,
                                       int key, int scancode,
                                       int action, int mods) {
            static_cast<Input*>(glfwGetWindowUserPointer(window))
                    ->keyboardInputCallback(key, action);
        }); // Set the keyboard input callback to this input callback
    }

    void keyboardInputCallback(const int& key, const int& action) {
        if (key >= 0 && key < KEY_COUNT) {
            if (action == GLFW_PRESS) m_keys_pressed[key] = true;
            else if (action == GLFW_RELEASE) m_keys_pressed[key] = false;
        }
    }
public:
    // Class is a singleton, so disable copy and assignment
    Input(Input const &) = delete;
    Input & operator=(Input const &) = delete;
private:
    bool m_keys_pressed[KEY_COUNT];
};
