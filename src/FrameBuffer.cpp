// Created by Modar Nasser on 26/11/2023.

#include <sogl/FrameBuffer.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


using namespace sogl;

FrameBuffer::FrameBuffer() : FrameBuffer(PostProcess(GLSL(330 core,
    uniform sampler2D texture0;
    vec4 postprocess(vec2 uv) {
        return texture(texture0, uv);
    }
))) {}

FrameBuffer::FrameBuffer(const PostProcess& post_process) {
    glGenTextures(1, &m_render_texture);
    glBindTexture(GL_TEXTURE_2D, m_render_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &m_depth_buffer);

    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_render_texture, 0);

    m_vertex_array.setPrimitiveType(Primitive::Triangles);
    m_vertex_array.pushQuad({{
        {{-1.f, 1.f}, {0.f, 1.f}},
        {{1.f, 1.f}, {1.f, 1.f}},
        {{1.f, -1.f}, {1.f, 0.f}},
        {{-1.f, -1.f}, {0.f, 0.f}}
    }});

    auto quad_vert_src = GLSL(330 core,
        precision highp float;
        layout(location = 0) in vec2 i_pos;
        layout(location = 1) in vec2 i_uv;

        out vec2 uv;

        void main(){
            gl_Position = vec4(i_pos.xy, 0., 1.);
            uv = i_uv;
        }
    );

    auto quad_frag_src = GLSL(330 core,
        precision highp float;
        uniform sampler2D texture0;
        uniform float time;
        uniform vec2 resolution;

        in vec2 uv;
        out vec4 color;

        vec4 postprocess(vec2 uv);

        void main(){
            vec4 c = postprocess(uv);
            color = c;
        }
    );

    auto vert_shader = Shader::compileVertex(quad_vert_src);
    auto frag_shader = Shader::compileFragment(quad_frag_src);
    auto postprocess_shader = Shader::compileFragment(post_process.fragment_src);

    m_shader.load(vert_shader, frag_shader, postprocess_shader);

    vert_shader.destroy();
    frag_shader.destroy();
    postprocess_shader.destroy();
}

FrameBuffer::~FrameBuffer() {
    glDeleteTextures(1, &m_render_texture);
    glDeleteRenderbuffers(1, &m_depth_buffer);
    glDeleteFramebuffers(1, &m_framebuffer);
}

void FrameBuffer::create(int width, int height) {
    m_size.x = width;
    m_size.y = height;

    glBindTexture(GL_TEXTURE_2D, m_render_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_size.x, m_size.y);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void FrameBuffer::clear(const glm::vec4& color) {
    glViewport(0, 0, m_size.x, m_size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::display() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::render() {
    m_shader.bind();
    m_shader.setUniform("time", static_cast<float>(glfwGetTime()));
    m_shader.setUniform("resolution", glm::vec2(m_size));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_render_texture);
    m_vertex_array.bind();
    m_vertex_array.render();
}
