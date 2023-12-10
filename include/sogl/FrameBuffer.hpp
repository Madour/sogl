// Created by Modar Nasser on 26/11/2023.

#pragma once

#include <sogl/PostProcess.hpp>
#include <sogl/Shader.hpp>
#include <sogl/VertexArray.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace sogl {

    class FrameBuffer {
    public:
        FrameBuffer();
        FrameBuffer(const PostProcess& post_process);
        ~FrameBuffer();
        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&&) = delete;

        void create(int width, int height);

        void clear(const glm::vec4& color = {0, 0, 0, 1});
        void display();

        void render();

    private:
        unsigned m_framebuffer = 0;
        unsigned m_render_texture = 0;
        unsigned m_depth_buffer = 0;

        VertexArray<glm::vec2, glm::vec2> m_vertex_array;

        glm::ivec2 m_size = {0, 0};
        Shader m_shader;
    };

}
