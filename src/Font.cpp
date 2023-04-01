// Created by Modar Nasser on 12/03/2023.

#include <sogl/Font.hpp>

#include <hb.h>
#include <hb-ft.h>
#include <freetype/freetype.h>

#include <vector>


using namespace sogl;

namespace {
    FT_Library ft_library = nullptr;
}

Font::~Font() {
    if (m_font)
        hb_font_destroy(static_cast<hb_font_t*>(m_font));

    if (m_face)
        FT_Done_Face(static_cast<FT_Face>(m_face));
}

auto Font::load(const std::filesystem::path& path) -> bool {
    if (ft_library == nullptr) {
        if (FT_Init_FreeType(&ft_library) != FT_Err_Ok)
            return false;
    }

    FT_Face ft_face;
    if (FT_New_Face(ft_library, path.string().c_str(), 0, &ft_face) != FT_Err_Ok)
        return false;

    m_face = ft_face;

    m_font = hb_ft_font_create_referenced(ft_face);

    return true;
}

void Font::shapeText(const std::string& string, int size, std::vector<Glyph::Shape>& glyphs) {
    hb_buffer_t* hb_buffer = hb_buffer_create();
    hb_buffer_add_utf8(hb_buffer, string.data(), -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);

    shape(hb_buffer, size, glyphs);

    hb_buffer_destroy(hb_buffer);
}

void Font::shapeText(const char8_t* string, int size, std::vector<Glyph::Shape>& glyphs) {
    hb_buffer_t* hb_buffer = hb_buffer_create();
    hb_buffer_add_utf8(hb_buffer, reinterpret_cast<const char*>(string), -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);

    shape(hb_buffer, size, glyphs);

    hb_buffer_destroy(hb_buffer);
}

void Font::shapeText(const std::wstring& string, int size, std::vector<Glyph::Shape>& glyphs) {
    hb_buffer_t* hb_buffer = hb_buffer_create();
    hb_buffer_add_utf16(hb_buffer, reinterpret_cast<const uint16_t*>(string.data()), -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);

    shape(hb_buffer, size, glyphs);

    hb_buffer_destroy(hb_buffer);
}

auto Font::getGlyph(std::uint32_t codepoint, int size) -> const Glyph& {
    auto ft_face = static_cast<FT_Face>(m_face);

    setPixelSize(size);

    if (m_glyphpages.count(size) == 0) {
        auto& glyphset = m_glyphpages[size];
        auto texture_size = size * 11;
        auto max_texture_size = Texture::getMaximumSize();
        glyphset.texture.create(std::min(texture_size, max_texture_size.x),
                                std::min(texture_size, max_texture_size.y));
        glyphset.next_pos.x = 0;
        glyphset.next_pos.y = 0;
    }

    auto& glyphpage = m_glyphpages[size];

    if (glyphpage.glyphs.count(codepoint) > 0) {
        return glyphpage.glyphs.at(codepoint);
    }
    else {
        FT_Load_Glyph(ft_face, codepoint, FT_LOAD_RENDER);
        auto& ft_glyph = ft_face->glyph;

        auto& glyph = glyphpage.glyphs[codepoint];
        glyph.advance = static_cast<float>(ft_glyph->advance.x) / 64.f;
        glyph.bearing.x = static_cast<float>(ft_glyph->metrics.horiBearingX) / 64.f;
        glyph.bearing.y = static_cast<float>(ft_glyph->metrics.horiBearingY) / 64.f;
        glyph.tex_pos.x = static_cast<float>(glyphpage.next_pos.x);
        glyph.tex_pos.y = static_cast<float>(glyphpage.next_pos.y);
        glyph.tex_size.x = static_cast<float>(ft_glyph->bitmap.width);
        glyph.tex_size.y = static_cast<float>(ft_glyph->bitmap.rows);

        assert(ft_glyph->bitmap.width == ft_glyph->bitmap.pitch);

        std::vector<std::uint8_t> pixels;
        pixels.reserve(static_cast<size_t>(glyph.tex_size.x * glyph.tex_size.y * 4));
        for (size_t i = 0; i < static_cast<size_t>(glyph.tex_size.x * glyph.tex_size.y); ++i) {
            pixels.push_back(0xff);
            pixels.push_back(0xff);
            pixels.push_back(0xff);
            pixels.push_back(ft_glyph->bitmap.buffer[i]);
        }

        glyphpage.texture.copy(
            pixels.data(),
            static_cast<int>(glyph.tex_pos.x),
            static_cast<int>(glyph.tex_pos.y),
            static_cast<int>(glyph.tex_size.x),
            static_cast<int>(glyph.tex_size.y)
        );

        glyphpage.next_pos.x += static_cast<int>(glyph.tex_size.x);
        if (glyphpage.next_pos.x + size >= glyphpage.texture.getSize().x) {
            glyphpage.next_pos.x = 0;
            glyphpage.next_pos.y += size;
        }

        return glyph;
    }
}

auto Font::getTexture(int size) const -> const Texture& {
    return m_glyphpages.at(size).texture;
}

void Font::shape(void* buffer, int size, std::vector<Glyph::Shape>& glyphs) {
    auto* hb_buffer = static_cast<hb_buffer_t*>(buffer);
    auto* hb_font = static_cast<hb_font_t*>(m_font);

    setPixelSize(size);

    hb_shape(hb_font, hb_buffer, nullptr, 0);

    unsigned int len = hb_buffer_get_length(hb_buffer);
    hb_glyph_info_t* info = hb_buffer_get_glyph_infos(hb_buffer, nullptr);
    hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(hb_buffer, nullptr);

    glyphs.resize(len);

    double curr_x = 0.;

    for (unsigned int i = 0; i < len; ++i) {
        auto& this_info = info[i];
        auto& this_pos = pos[i];
        auto& glyph = getGlyph(this_info.codepoint, m_last_size);

        double pos_x = curr_x + this_pos.x_offset / 64. + glyph.bearing.x;
        double pos_y = this_pos.y_offset / 64. - glyph.bearing.y;
        double adv_x = this_pos.x_advance / 64.;
        double adv_y = this_pos.y_advance / 64.;

        double tex_x = glyph.tex_pos.x;
        double tex_y = glyph.tex_pos.y;
        double tex_w = glyph.tex_size.x;
        double tex_h = glyph.tex_size.y;

        glyphs[i].x = static_cast<float>(pos_x);
        glyphs[i].y = static_cast<float>(pos_y);
        glyphs[i].u = static_cast<float>(tex_x);
        glyphs[i].v = static_cast<float>(tex_y);
        glyphs[i].w = static_cast<float>(tex_w);
        glyphs[i].h = static_cast<float>(tex_h);

        curr_x += adv_x;
    }
}

void Font::setPixelSize(int size) {
    if (size != m_last_size)
    {
        FT_Set_Pixel_Sizes(static_cast<FT_Face>(m_face), size, size);
        m_last_size = size;
    }
}
