// Created by Modar Nasser on 05/03/2022.

#include <sogl/Texture.hpp>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

auto Texture::load(const std::filesystem::path& file) -> bool {
    auto* texture_data = stbi_load(file.string().c_str(), &m_size.x, &m_size.y, &m_chan_count, 0);

    if (texture_data == nullptr)
        return false;

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    return true;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

auto Texture::getSize() const -> const glm::vec<2, int>& {
    return m_size;
}

auto Texture::getChannelsCount() const -> int {
    return m_chan_count;
}
