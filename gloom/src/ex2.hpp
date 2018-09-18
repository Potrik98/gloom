#pragma once

#include "task.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vao.hpp"
#include "geometry.hpp"

#include "math.h"

namespace ex2 {
    class Task1 : public BaseTask {
    public:
        void init() {
            // Create the shader program
            m_shader.makeBasicShader("../gloom/shaders/color.vert",
                                     "../gloom/shaders/color.frag");
            init_vao();
        }
    protected:
        void init_vao() {
            m_vao = geometry::gen_circle_vao(
                0.0f, // center x
                0.0f, // center y
                0.5f, // radius
                5 // corners
            );
            const unsigned int component_count = 4;
            const unsigned int value_count = m_vao.getVertexCount() * component_count;
            float colors[value_count];
            for (int i = 0; i < m_vao.getVertexCount(); ++i) {
                float r = 0.5f + 0.5f * cosf(i * 2.1f);
                float g = 0.5f + 0.5f * cosf(i * 1.7f + 0.2f);
                float b = 0.5f + 0.5f * cosf(i * 0.5f + 2.7f);
                float a = 1.0f;
                colors[component_count * i + 0] = r;
                colors[component_count * i + 1] = g;
                colors[component_count * i + 2] = b;
                colors[component_count * i + 3] = a;
            }
            m_vao.vertexArray(colors, component_count, 1); // Add colors to location 1
        }
    };
}
