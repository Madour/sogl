// Created by Modar Nasser on 26/11/2023.

#include <sogl/FrameBuffer.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


using namespace sogl;

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &m_framebuffer);
    glGenTextures(1, &m_render_texture);
    glGenRenderbuffers(1, &m_depth_buffer);

    m_vertex_array.setPrimitiveType(Primitive::Triangles);
    m_vertex_array.pushQuad({{
        {{-1.f, 1.f}, {0.f, 1.f}},
        {{1.f, 1.f}, {1.f, 1.f}},
        {{1.f, -1.f}, {1.f, 0.f}},
        {{-1.f, -1.f}, {0.f, 0.f}}
    }});
}

FrameBuffer::~FrameBuffer() {
    glDeleteTextures(1, &m_render_texture);
    glDeleteRenderbuffers(1, &m_depth_buffer);
    glDeleteFramebuffers(1, &m_framebuffer);
}

void FrameBuffer::create(int width, int height) {
    m_size.x = width;
    m_size.y = height;

    // create render texture
    glBindTexture(GL_TEXTURE_2D, m_render_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create depth render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_size.x, m_size.y);

    // attach render texture and depth render buffer to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_render_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::clear(const glm::vec4& color) {
    glViewport(0, 0, m_size.x, m_size.y);
    glClearColor(color.r, color.g, color.b, color.a);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void FrameBuffer::display() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::render() const {
    render(PostProcess::getDefault());
}

void FrameBuffer::render(const PostProcess& post_process,
                         const std::map<std::string, float>& uniforms) const {
    post_process.shader.bind();
    post_process.shader.setUniform("time", static_cast<float>(glfwGetTime()));
    post_process.shader.setUniform("resolution", glm::vec2(m_size));
    for (const auto& [name, val] : uniforms) {
        post_process.shader.setUniform(name, val);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_render_texture);
    m_vertex_array.bind();
    m_vertex_array.render();
}
