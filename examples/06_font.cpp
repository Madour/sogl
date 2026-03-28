#include <sogl/sogl.hpp>

#include <codecvt>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

auto convertUtf8ToUtf16(const std::string& utf8) -> std::wstring {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8);
}

void loadTextShader(sogl::Shader& shader) {
    auto vert_src = GLSL(330 core,
                         precision highp float;
                         uniform vec2 tex_size;
                         layout (location = 0) in vec2 i_pos;
                         layout (location = 1) in vec2 i_tex;
                         out vec2 tex;
                         void main() {
                             tex = i_tex / tex_size;
                             float pos_x = i_pos.x * 2. / 1280. - 1.;
                             float pos_y = i_pos.y * 2. / 720. - 1.;
                             gl_Position = vec4(pos_x, -pos_y, 0, 1.0);
                         }
                    );
    auto frag_src = GLSL(330 core,
                         precision highp float;
                         uniform sampler2D texture0;
                         in vec2 tex;
                         out vec4 fragColor;
                         void main() {
                             vec4 col = texture(texture0, tex);
                             fragColor = col;
                         }
                    );
    auto vert_shader = sogl::Shader::compileVertex(vert_src);
    auto frag_shader = sogl::Shader::compileFragment(frag_src);
    shader.load(vert_shader, frag_shader);
    vert_shader.destroy();
    frag_shader.destroy();
}

void loadDebugShader(sogl::Shader& shader) {
    auto vert_src = GLSL(330 core,
                         precision highp float;
                         layout (location = 0) in vec2 i_pos;
                         void main() {
                             float pos_x = i_pos.x * 2. / 1280. - 1.;
                             float pos_y = i_pos.y * 2. / 720. - 1.;
                             gl_Position = vec4(pos_x, -pos_y, 0, 1.0);
                         }
                    );
    auto frag_src = GLSL(330 core,
                         precision highp float;
                         out vec4 fragColor;
                         void main() {
                             fragColor = vec4(1, 0, 0, 1);
                         }
                    );
    auto vert_shader = sogl::Shader::compileVertex(vert_src);
    auto frag_shader = sogl::Shader::compileFragment(frag_src);
    shader.load(vert_shader, frag_shader);
    vert_shader.destroy();
    frag_shader.destroy();
}

struct Text {
    std::wstring string;
    bool is_rtl = false;
    sogl::Font* font;
    size_t font_size = 30;
    sogl::VertexArray<glm::vec2, glm::vec2> vertex_array;
    sogl::VertexArray<glm::vec2, glm::vec2> font_va;
    sogl::VertexArray<glm::vec2> glyph_bounds_va;
};

int main(int argc, char **argv)
{
    auto window = sogl::Window(1280, 720, "Font");

    auto text_shader = sogl::Shader();
    loadTextShader(text_shader);

    auto debug_shader = sogl::Shader();
    loadDebugShader(debug_shader);

    bool render_glyph_bounds = false;

    auto font_default = sogl::Font();
    font_default.load("06-assets/NotoSerif.ttf");

    auto font_arabic = sogl::Font();
    font_arabic.load("06-assets/UthmanicScriptHafs.otf");

    auto font_japanese = sogl::Font();
    font_japanese.load("06-assets/NotoSerifJp.ttf");

    auto font_devanagiri = sogl::Font();
    font_devanagiri.load("06-assets/NotoSerifDevanagari.ttf");

    auto font_georgian = sogl::Font();
    font_georgian.load("06-assets/NotoSerifGeorgian.ttf");

    auto font_urdu = sogl::Font();
    font_urdu.load("06-assets/JameelNooriNastaleeq.ttf");

    std::vector<std::unique_ptr<Text>> texts;
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("هلّا سَكَنَت بِذي ضغثٍ فقد رَغِموا ... شخصت تطلِبُ ظَبِياً راحَ مجتازا");
        text->is_rtl = true;
        text->font = &font_arabic;
        text->font_size += 4;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("Voix ambiguë d’un cœur qui au zéphyr préfère les jattes de kiwis.");
        text->font = &font_default;
        text->font_size -= 1;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("色は匂えど散りぬるを我が世誰ぞ常ならん");
        text->font = &font_japanese;
        text->font_size += 2;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("द्रुतगतिः शृगालः आलस्यस्य श्वः उपरि कूर्दति");
        text->font = &font_devanagiri;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("Съешь ещё этих мягких французских булок да выпей же чаю");
        text->font = &font_default;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("აბგად ევზეთ იკალ მანო, პაჟა რასტა უფქა ღაყაშ, ჩაცა ძაწა ჭახა ჯაჰო");
        text->font = &font_georgian;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("ٹھنڈ میں، ایک قحط زدہ گاؤں سے گذرتے وقت ایک چڑچڑے، باأثر و فارغ شخص کو بعض جل پری نما اژدہے نظر آئے");
        text->is_rtl = true;
        text->font = &font_urdu;
        text->font_size += 6;
        texts.emplace_back(text);
    }
    {
        auto* text = new Text();
        text->string = convertUtf8ToUtf16("Press F1 to toggle glyph bounds");
        text->font = &font_default;
        text->font_size = 20;
        texts.emplace_back(text);
    }

    auto cy = 0;
    for (auto& text : texts) {
        std::vector<sogl::Glyph::Shape> glyph_shapes;
        text->font->shapeText(text->string, text->font_size, glyph_shapes);

        cy += 80;
        auto cx = 50;
        if (text->is_rtl) {
            auto& last_glyph = glyph_shapes[glyph_shapes.size()-1];
            auto min = glyph_shapes[0].x;
            auto max = last_glyph.x + last_glyph.w;
            auto width = max - min;
            cx = window.getSize().x - int(width) - cx;
        }

        text->vertex_array.setPrimitiveType(sogl::Primitive::Triangles);
        text->glyph_bounds_va.setPrimitiveType(sogl::Primitive::Lines);
        for (auto& [x, y, u, v, w, h] : glyph_shapes) {
            text->vertex_array.pushQuad({{
                // vertex pos               // texture pos
                // x          y              u      v
                {{cx + x,     cy + y},      {u,     v}},
                {{cx + x + w, cy + y},      {u + w, v}},
                {{cx + x + w, cy + y + h},  {u + w, v + h}},
                {{cx + x,     cy + y + h},  {u,     v + h}}
            }});

            text->glyph_bounds_va.push({{cx + x,     cy + y}});
            text->glyph_bounds_va.push({{cx + x + w, cy + y}});
            text->glyph_bounds_va.push({{cx + x + w, cy + y}});
            text->glyph_bounds_va.push({{cx + x + w, cy + y + h}});
            text->glyph_bounds_va.push({{cx + x + w, cy + y + h}});
            text->glyph_bounds_va.push({{cx + x,     cy + y + h}});
            text->glyph_bounds_va.push({{cx + x,     cy + y + h}});
            text->glyph_bounds_va.push({{cx + x,     cy + y}});
        }

        auto texture_size = text->font->getTexture(text->font_size).getSize();
        auto tex_w = texture_size.x;
        auto tex_h = texture_size.y;
        text->font_va.pushQuad({{
            {{15,         15},         {0, 0}},
            {{15 + tex_w, 15},         {tex_w, 0}},
            {{15 + tex_w, 15 + tex_h}, {tex_w, tex_h}},
            {{15,         15 + tex_w}, {0, tex_w}}
        }});
    }

    auto update_fn = [&]() {
        while (auto event = window.nextEvent()) {
            if (auto e = event->as<sogl::Event::KeyPress>()) {
                if (e->key == sogl::Key::Escape) {
                    window.close();
                }
                else if (e->key == sogl::Key::F1) {
                    render_glyph_bounds = !render_glyph_bounds;
                }
            }
        }
    };
    auto render_fn = [&]() {
        window.clear({0.6, 0.4, 0.8});

        glDisable(GL_DEPTH_TEST);
        for (auto& text : texts) {
            auto& font_tex = text->font->getTexture(text->font_size);
            font_tex.bind();
            text_shader.bind();
            text_shader.setUniform("tex_size", glm::vec2(font_tex.getSize()));
            text->vertex_array.bind();
            text->vertex_array.render();

            if (render_glyph_bounds) {
                debug_shader.bind();
                text->glyph_bounds_va.bind();
                text->glyph_bounds_va.render();
            }
        }
        //text_shader.bind();
        //texts.back()->font_va.bind();
        //texts.back()->font_va.render();
        glEnable(GL_DEPTH_TEST);

        window.display();
    };

#if !defined(EMSCRIPTEN)
    while (window.isOpen())
    {
        update_fn();
        render_fn();
    }
#else
    struct App {
        std::function<void()> update;
        std::function<void()> render;
    };
    auto app = App{update_fn, render_fn};
    auto main_loop = [] (void* arg) {
        auto* app = static_cast<App*>(arg);
        app->update();
        app->render();
    };
    // start emscripten main loop
    emscripten_set_main_loop_arg(main_loop, &app, 0, EM_TRUE);
#endif
    return 0;
}
