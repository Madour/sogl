// Created by Modar Nasser on 12/03/2022.

#include <sogl/sogl.hpp>

int main() {
    // create a window
    auto window = sogl::Window(1000, 800, "Triangle demo");

    // create a vertex array and push a triangle
    // vertex data is : vec2 position, vec2 texture_coordinates, vec4 color
    auto vertex_array = sogl::VertexArray();
    vertex_array.pushTriangle({{
        {{0.f, -0.5f}, {}, {1.f, 0.f, 0.f, 1.f}},
        {{0.5f, 0.5f}, {}, {0.f, 1.f, 0.f, 1.f}},
        {{-0.5f, 0.5f}, {}, {0.f, 0.f, 1.f, 1.f}}
    }});

    // basic vertex shader, its inputs must respect this layout
    auto vert_src = GLSL(330 core,
        precision highp float;
        layout (location = 0) in vec2 i_pos;
        layout (location = 1) in vec2 i_tex;
        layout (location = 2) in vec4 i_col;

        out vec4 col;

        void main() {
            col = i_col;
            gl_Position = vec4(i_pos.x, -i_pos.y, 0, 1.0);
        }
    );
    // basic fragment shader
    auto frag_src = GLSL(330 core,
        precision highp float;
        in vec4 col;
        out vec4 fragColor;

        void main() {
            fragColor = col;
        }
    );
    // create a shader program and load it from the source strings
    auto shader = sogl::Shader();
    shader.load(vert_src, frag_src);

#if !defined(EMSCRIPTEN)
    // start main loop
    while (window.isOpen()) {
        window.clear();
        shader.bind();
        vertex_array.bind();
        vertex_array.render();
        window.display();
    }
#else
    struct TriangleExampleContext {
        sogl::Window& window;
        sogl::Shader& shader;
        sogl::VertexArray& vertex_array;
    };
    auto app_ctx = TriangleExampleContext{window, shader, vertex_array};
    auto main_loop = [] (void* arg) {
        auto* ctx = static_cast<TriangleExampleContext*>(arg);
        ctx->window.clear();
        ctx->shader.bind();
        ctx->vertex_array.bind();
        ctx->vertex_array.render();
        ctx->window.display();
    };
    // start emscripten main loop
    emscripten_set_main_loop_arg(main_loop, &app_ctx, 0, EM_TRUE);
#endif
    return 0;
}