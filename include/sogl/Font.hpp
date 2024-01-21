// Created by Modar Nasser on 12/03/2023.

#pragma once

#include <sogl/Texture.hpp>

#include <glm/vec2.hpp>

#include <filesystem>
#include <unordered_map>


namespace sogl {

    struct Glyph {
        float advance = 0;
        glm::vec2 bearing;
        glm::vec2 tex_pos;
        glm::vec2 tex_size;

        struct Shape {
            float x, y;
            float u, v;
            float w, h;
        };
    };

    class Font {
    public:
        Font() = default;
        Font(const Font&) = delete;
        Font(Font&&) = delete;
        Font& operator=(const Font&) = delete;
        Font& operator=(const Font&&) = delete;

        ~Font();

        auto load(const std::filesystem::path& path) -> bool;

        void shapeText(const std::string& string, int size, std::vector<Glyph::Shape>& glyphs);
        void shapeText(const char8_t* string, int size, std::vector<Glyph::Shape>& glyphs);
        void shapeText(const std::wstring& string, int size, std::vector<Glyph::Shape>& glyphs);

        auto getGlyph(std::uint32_t codepoint, int size) -> const Glyph&;

        [[nodiscard]] auto getTexture(int size) const -> const Texture&;

    private:
        void setPixelSize(int size);
        void shape(void* buffer, int size, std::vector<Glyph::Shape>& vertices);

        struct GlyphPage {
            std::unordered_map<std::uint32_t, Glyph> glyphs;
            Texture texture;
            glm::vec<2, int> next_pos;
        };

        void* m_face = nullptr;
        void* m_font = nullptr;
        int m_last_size = 16;
        std::unordered_map<int, GlyphPage> m_glyphpages;
    };

}
