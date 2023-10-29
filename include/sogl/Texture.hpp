// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <glm/vec2.hpp>

#include <filesystem>

namespace sogl {

    class Texture {
    public:
        Texture();
        ~Texture();
        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(const Texture&&) = delete;

        void create(int width, int height);

        auto load(const std::filesystem::path& file) -> bool;

        auto copy(const std::uint8_t* pixels, int x, int y, int width, int height) -> bool;

        void bind() const;

        static void unbind();

        [[nodiscard]] auto getSize() const -> const glm::vec<2, int>&;

        [[nodiscard]] static auto getMaximumSize() -> glm::vec<2, int>;

    private:
        unsigned m_texture = 0;
        glm::vec<2, int> m_size = {0, 0};
    };

}
