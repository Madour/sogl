// Created by Modar Nasser on 05/03/2022.

#include <sogl/Texture.hpp>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "thirdparty/stb_image_write.h"

using namespace sogl;

Texture::Texture() {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_texture);
}

void Texture::create(int width, int height) {
    m_size.x = width;
    m_size.y = height;

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

auto Texture::load(const std::filesystem::path& file) -> bool {
    int width = 0;
    int height = 0;
    int chan_count = 0;
    auto* pixels = stbi_load(file.string().c_str(), &width, &height, &chan_count, 4);

    if (pixels == nullptr)
        return false;

    create(width, height);

    auto ret = copy(pixels, 0, 0, width, height);

    stbi_image_free(pixels);

    return ret;
}

auto Texture::copy(const std::uint8_t* pixels, int x, int y, int width, int height) -> bool {
    if (pixels == nullptr)
        return false;

    if (m_size.x == 0 && m_size.y == 0) {
        create(x + width, y + height);
    }

    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::getSize() const -> const glm::vec<2, int>& {
    return m_size;
}

auto Texture::getMaximumSize() -> glm::vec<2, int> {
    GLint size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
    return {size, size};
}
