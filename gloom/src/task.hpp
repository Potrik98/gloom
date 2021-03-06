#pragma once

#include "vao.hpp"
#include "gloom/shader.hpp"

#include <vector>

/*
 * Abstract class for tasks.
 * All tasks are initializable and renderable.
 */
class Task {
public:
    /*
     * Initialize task variables
     */
    virtual void init() = 0;

    /*
     * Render task elements
     */
    virtual void render() = 0;
};

/*
 * Base task is an abstract class for all basic tasks,
 * and handles rendering, and initalization of the default
 * simple shader.
 * All base tasks must have a vao to render:
 * the abstract init_vao method initializes the vao object. 
 */
class BaseTask : public Task {
public:
    void init() {
        // Create a basic shader program
        m_shader.makeBasicShader("../gloom/shaders/simple.vert",
                                 "../gloom/shaders/simple.frag");
        // Initialize the vao
        init_vao();
    }

    void render() {
        // Activate the shader program and render the vao
        m_shader.activate();
        m_vao.render();
    }
protected:
    VertexArrayObject m_vao;
    Gloom::Shader m_shader;

    /*
     * Abstract method.
     * All implementations must initialize a vao.
     */
    virtual void init_vao() = 0;
};

/*
 * Advanced task is an abstract class for more advanced tasks,
 * containing more than one vao.
 * The class handles rendering, and initalization of the default
 * simple shader.
 * All implementations must have initialize their own vaos.
 * the abstract init_vaos method initializes the vaos. 
 */
class AdvancedTask : public Task {
public:
    void init() {
        // Create a basic shader program
        m_shader.makeBasicShader("../gloom/shaders/simple.vert",
                                 "../gloom/shaders/simple.frag");
        // Initialize the vao
        init_vaos();
    }

    void render() {
        // Activate the shader program and render the vao
        m_shader.activate();
        for (VertexArrayObject vao : m_vaos) { vao.render(); }
    }
protected:
    std::vector<VertexArrayObject> m_vaos;
    Gloom::Shader m_shader;

    /*
     * Abstract method.
     * All implementations must initialize their vaos.
     */
    virtual void init_vaos() = 0;
};
