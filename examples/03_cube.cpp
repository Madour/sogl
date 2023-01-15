// Created by Modar Nasser on 14/01/2023.

#include <sogl/sogl.hpp>

#include <glm/gtc/matrix_transform.hpp>


int main() {
    // create a window
    auto window = sogl::Window(800, 600, "Triangle demo");

    const auto red = glm::vec4(1.f, 0.f, 0.f, 1.f);
    const auto green = glm::vec4(0.f, 1.f, 0.f, 1.f);
    const auto blue = glm::vec4(0.f, 0.f, 1.f, 1.f);
    const auto yellow = glm::vec4(1.f, 1.f, 0.f, 1.f);
    const auto cyan = glm::vec4(0.f, 1.f, 1.f, 1.f);
    const auto magenta = glm::vec4(1.f, 0.f, 1.f, 1.f);

    auto cube = sogl::VertexArray<glm::vec3, glm::vec4>(sogl::Primitive::Triangles);
    // bottom
    cube.pushQuad({{
           {{-1, -1, -1}, magenta},
           {{1, -1, -1}, magenta},
           {{1, -1, 1}, magenta},
           {{-1, -1, 1}, magenta}
    }});
    // top
    cube.pushQuad({{
           {{-1, 1, -1}, red},
           {{1, 1, -1}, red},
           {{1, 1, 1}, red},
           {{-1, 1, 1}, red}
    }});
    // left
    cube.pushQuad({{
           {{-1, -1, -1}, cyan},
           {{-1, -1, 1}, cyan},
           {{-1, 1, 1}, cyan},
           {{-1, 1, -1}, cyan}
    }});
    // right
    cube.pushQuad({{
           {{1, -1, -1}, blue},
           {{1, -1, 1}, blue},
           {{1, 1, 1}, blue},
           {{1, 1, -1}, blue}
    }});
    // back
    cube.pushQuad({{
           {{-1, -1, -1}, yellow},
           {{1, -1, -1}, yellow},
           {{1, 1, -1}, yellow},
           {{-1, 1, -1}, yellow}
    }});
    // front
    cube.pushQuad({{
           {{-1, -1, 1}, green},
           {{1, -1, 1}, green},
           {{1, 1, 1}, green},
           {{-1, 1, 1}, green}
    }});

    auto axis = sogl::VertexArray<glm::vec3, glm::vec4>(sogl::Primitive::Lines);
    axis.pushQuad({{
            {{0, 0, 0}, red},
            {{50, 0, 0}, red},
    }});
    axis.pushQuad({{
            {{0, 0, 0}, green},
            {{0, 50, 0}, green},
    }});
    axis.pushQuad({{
            {{0, 0, 0}, blue},
            {{0, 0, 50}, blue},
    }});

    // vertex shader with projection view matrix calculation
    auto vert_src = GLSL(330 core,
         precision highp float;
         uniform mat4 projection;
         uniform mat4 camera;
         uniform mat4 model;

         layout (location = 0) in vec3 i_pos;
         layout (location = 1) in vec4 i_col;

         out vec4 col;

         void main() {
             col = i_col;
             gl_Position = projection * camera * model * vec4(i_pos.xyz, 1.0);
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

    auto view_size = glm::vec2(window.getSize());

    glm::mat4 projection = glm::perspective(glm::radians(50.0f), view_size.x/view_size.y, 0.1f, 50.0f);
    glm::mat4 camera = glm::lookAt(glm::vec3(5, 3, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    float angle = 0.f;
    unsigned dt_ms = 0;

    auto update_fn = [&]() {
        while (const auto& event = window.nextEvent()) {
            if (const auto& resize = event->as<sogl::Event::Resize>()) {
                auto new_size = glm::vec2(window.getSize());
                projection = glm::perspective(glm::radians(50.0f), new_size.x/new_size.y, 0.1f, 50.0f);
            }
        }

        angle += 120.f * static_cast<float>(dt_ms) / 1000.f;
    };

    auto render_fn = [&]() {
        window.clear();

        auto model = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), glm::vec3(0, 1, 0));

        shader.bind();
        shader.setUniform("projection", projection);
        shader.setUniform("camera", camera);
        shader.setUniform("model", model);
        cube.bind();
        cube.render();

        shader.bind();
        shader.setUniform("projection", projection);
        shader.setUniform("camera", camera);
        shader.setUniform("model", glm::identity<glm::mat4>());
        axis.bind();
        axis.render();

        dt_ms = window.display();
    };

#if !defined(EMSCRIPTEN)
    // start main loop
    while (window.isOpen()) {
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