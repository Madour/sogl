// Created by Modar Nasser on 10/12/2022.

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

    // basic vertex shader, its inputs layout must respect the vertex array data
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

    // let's create some post processing effects
    auto waves_post_process = sogl::PostProcess(GLSL(330 core,
        // uniforms are provided by the framebuffer
        uniform sampler2D texture0;
        uniform float time;
        uniform vec2 resolution;

        vec4 postprocess(vec2 uv) {
            vec2 offset = 0.08 * vec2(cos(time + uv.y * 24), 0);
            return texture(texture0, uv + offset);
        }
    ));

    auto wiggle_post_process = sogl::PostProcess(GLSL(330 core,
        uniform sampler2D texture0;
        uniform float time;
        uniform vec2 resolution;

        vec4 postprocess(vec2 uv) {
            vec2 offset = 0.005 * vec2(sin(time + resolution.x * uv.x), cos(time + resolution.y * uv.y));
            return texture(texture0, uv + offset);
        }
    ));

    // gaussian blur post process (from https://www.shadertoy.com/view/XdfGDH)
    auto blur_post_process = sogl::PostProcess(GLSL(330 core,
        uniform sampler2D texture0;
        uniform float time;
        uniform vec2 resolution;

        float normpdf(in float x, in float sigma) {
            return 0.39894 * exp((-0.5 * x * x) / (sigma * sigma)) / sigma;
        }

        vec4 postprocess(vec2 uv) {
            const int blur_radius = 5;
            const int mSize = 2 * blur_radius + 1;
            const int kSize = (mSize - 1) / 2;
            float kernel[mSize];

            float sigma = 10.0;
            for (int j = 0; j <= kSize; ++j) {
                kernel[kSize + j] = kernel[kSize - j] = normpdf(j, sigma);
            }

            float Z = 0.0;
            for (int j = 0; j < mSize; ++j) {
                Z += kernel[j];
            }

            vec3 final_color = vec3(0.0);
            for (int i = -kSize; i <= kSize; ++i) {
                for (int j = -kSize; j <= kSize; ++j) {
                    vec3 texel = texture(texture0, uv + vec2(i, j) / resolution).rgb;
                    final_color += kernel[kSize + j] * kernel[kSize + i] * texel;
                }
            }

            return vec4(final_color / (Z * Z), 1.0);
        }
    ));

    // create the framebuffer that will render the scene and apply the post processing effect
    auto framebuffer = sogl::FrameBuffer();
    framebuffer.create(window.getSize().x, window.getSize().y);

    const sogl::PostProcess* current_post_process = &waves_post_process;

    auto update_fn = [&]() {
        while (auto event = window.nextEvent()) {
            if (auto key_press = event->as<sogl::Event::KeyPress>()) {
                if (key_press->key == sogl::Key::Escape) {
                    window.close();
                }
                else {
                    static size_t i = 1;
                    i = (i + 1) % 4;
                    switch (i) {
                        case 0: current_post_process = &sogl::PostProcess::getDefault(); break;
                        case 1: current_post_process = &waves_post_process; break;
                        case 2: current_post_process = &wiggle_post_process; break;
                        case 3: current_post_process = &blur_post_process; break;
                    }
                }
            }
        }
    };

    auto render_fn = [&]() {
        // render the scene to the framebuffer first
        framebuffer.clear();
        shader.bind();
        vertex_array.bind();
        vertex_array.render();
        framebuffer.display();

        // then render the framebuffer to the window
        window.clear();
        framebuffer.render(*current_post_process);
        window.display();
    };

#if !defined(EMSCRIPTEN)
    // start main loop
    while (window.isOpen()) {
        update_fn();
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
