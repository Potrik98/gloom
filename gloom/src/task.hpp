#pragma once

#include "vao.hpp"
#include "gloom/shader.hpp"

class Task {
public:
    virtual void init() = 0;
    virtual void render() = 0;
};

class BaseTask : public Task {
public:
    void init() {
        // Create the shader program
        m_shader.makeBasicShader("../gloom/shaders/simple.vert",
                                 "../gloom/shaders/simple.frag");
        init_vao();
    }

    void render() {
        m_shader.activate();
        m_vao.render();
    }
protected:
    virtual void init_vao() = 0;
    VertexArrayObject m_vao;
    Gloom::Shader m_shader;
};
