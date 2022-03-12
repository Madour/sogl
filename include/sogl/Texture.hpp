// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <glm/glm.hpp>

#include <filesystem>

namespace sogl {

    class Texture {
    public:
        Texture();
        void load(const std::filesystem::path& file);

        void bind() const;

        [[nodiscard]] auto getSize() const -> const glm::vec<2, int>&;
        [[nodiscard]] auto getChannelsCount() const -> int;

    private:
        unsigned m_texture = 0;
        glm::vec<2, int> m_size = {0, 0};
        int m_chan_count = 0;
    };

}
