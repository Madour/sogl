// Created by Modar Nasser on 12/03/2022.

#include <sogl/sogl.hpp>

int main() {
    // create a window
    auto options = sogl::Window::Options();
    options.v_sync = true;
    auto window = sogl::Window(1000, 800, "Triangle demo", options);

    // create a vertex array and push a triangle
    // vertex data is : vec2 position, vec4 color
    auto vertex_array = sogl::VertexArray<glm::vec2, glm::vec4>(sogl::Primitive::Triangles);
    vertex_array.pushTriangle({{
        {{0.f, 0.5f}, {1.f, 0.f, 0.f, 1.f}},
        {{0.5f, -0.5f}, {0.f, 1.f, 0.f, 1.f}},
        {{-0.5f, -0.5f}, {0.f, 0.f, 1.f, 1.f}}
    }});

    // basic vertex shader, its inputs must respect the vertex array data
    auto vert_src = GLSL(330 core,
        precision highp float;
        layout (location = 0) in vec2 i_pos;
        layout (location = 1) in vec4 i_col;

        out vec4 col;

        void main() {
            col = i_col;
            gl_Position = vec4(i_pos.xy, 0, 1.0);
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

    // compile shaders from source string
    auto vert_shader = sogl::Shader::compileVertex(vert_src);
    auto frag_shader = sogl::Shader::compileFragment(frag_src);

    // create a shader program and load it from the compiled shaders
    auto shader = sogl::Shader();
    shader.load(vert_shader, frag_shader);

    // once the shader program is loaded, we can destroy the compiled shaders
    vert_shader.destroy();
    frag_shader.destroy();

    auto render_fn = [&]() {
        window.clear();
        shader.bind();
        vertex_array.bind();
        vertex_array.render();
        window.display();
    };

#if !defined(EMSCRIPTEN)
    // start main loop
    while (window.isOpen()) {
        render_fn();
    }
#else
    struct App {
        std::function<void()> render;
    };
    auto app = App{render_fn};
    auto main_loop = [] (void* arg) {
        auto* app = static_cast<App*>(arg);
        app->render();
    };
    // start emscripten main loop
    emscripten_set_main_loop_arg(main_loop, &app, 0, EM_TRUE);
#endif
    return 0;
}
