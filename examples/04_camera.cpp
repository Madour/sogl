// Created by Modar Nasser on 14/01/2023.

// This example uses a left-handed coordinates system with the Z axis pointing upwards
#define GLM_FORCE_LEFT_HANDED

#include <sogl/sogl.hpp>

#include <glm/gtc/matrix_transform.hpp>


struct Camera {
    const float fov = 45.f;
    const float near = 0.01f;
    const float far = 6000.f;
    const glm::vec3 up = {0, 0, 1};

    glm::vec3 pos = {0, 0, 0};
    glm::vec3 dir = {1, 1, 0};
    glm::vec3 rot = {0, 0, 0};

    inline void update(const glm::vec2& new_size) {
        projection = glm::perspective(glm::radians(fov), new_size.x/new_size.y, near, far);
    }

    [[nodiscard]] inline auto matrix() const -> glm::mat4 {
        return projection * glm::lookAt(pos, pos + dir, up);
    }

private:
    glm::mat4 projection{};
};

int main() {
    // create a window
    auto window = sogl::Window(800, 600, "3D camera demo");
    // and hide the cursor
    glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
       {{5000, 0, 0}, red},
   }});
    axis.pushQuad({{
       {{0, 0, 0}, green},
       {{0, 5000, 0}, green},
   }});
    axis.pushQuad({{
       {{0, 0, 0}, blue},
       {{0, 0, 5000}, blue},
   }});

    // basic vertex shader, its inputs must respect the vertex array data
    auto vert_src = GLSL(330 core,
         precision highp float;
         uniform mat4 camera;
         uniform mat4 model;

         layout (location = 0) in vec3 i_pos;
         layout (location = 1) in vec4 i_col;

         out vec4 col;

         void main() {
             col = i_col;
             gl_Position = camera * model * vec4(i_pos.xyz, 1.0);
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

    Camera cam{};
    cam.pos = {10, 10, 2};
    cam.dir = {0, -1, 0};
    cam.rot = {0, 0, glm::radians(-136.f)};
    cam.update(glm::vec2(window.getSize()));

    glm::vec2 last_mouse_pos = window.getMousePosition();

    float cube_rotation = 0.f;
    float dt = 0;

    auto update_fn = [&]() {
        while (const auto& event = window.nextEvent()) {
            if (const auto& resize = event->as<sogl::Event::Resize>()) {
                auto new_size = glm::vec2(window.getSize());
                cam.update(new_size);
            }

            else if (const auto& press = event->as<sogl::Event::KeyPress>()) {
                if (press->key == sogl::Key::Escape) {
#if !defined(EMSCRIPTEN)
                    window.close();
#endif
                }
            }
        }

        {
            // move camera based on keyboard inputs
            constexpr float MOVE_SPEED = 10.f;

            if (window.isKeyPressed(sogl::Key::W)) {
                cam.pos += cam.dir * MOVE_SPEED * dt;
            }
            if (window.isKeyPressed(sogl::Key::S)) {
                cam.pos -= cam.dir * MOVE_SPEED * dt;
            }
            if (window.isKeyPressed(sogl::Key::D)) {
                cam.pos += glm::cross(-cam.dir, cam.up) * MOVE_SPEED * dt;
            }
            if (window.isKeyPressed(sogl::Key::A)) {
                cam.pos -= glm::cross(-cam.dir, cam.up) * MOVE_SPEED * dt;
            }
            if (window.isKeyPressed(sogl::Key::Space)) {
                cam.pos += cam.up * MOVE_SPEED * dt;
            }
            if (window.isKeyPressed(sogl::Key::LeftShift)) {
                cam.pos -= cam.up * MOVE_SPEED * dt;
            }
        }

        {
            //rotate camera based on mouse movement
            constexpr float MOUSE_SENSITIVITY = 0.1f;
            auto mouse_pos = glm::vec2(window.getMousePosition());

            auto angles_offset = (mouse_pos - last_mouse_pos) * MOUSE_SENSITIVITY;
            angles_offset.y = -angles_offset.y;
            last_mouse_pos = mouse_pos;

            cam.rot.x += glm::radians(angles_offset.y); // pitch
            cam.rot.z += glm::radians(angles_offset.x); // yaw

            if (cam.rot.x < glm::radians(-89.f)) cam.rot.x = glm::radians(-89.f);
            if (cam.rot.x > glm::radians(89.f)) cam.rot.x = glm::radians(89.f);

            // update camera direction according to the mouse movement
            glm::vec3 direction;
            direction.x = std::cos(cam.rot.z) * std::cos(cam.rot.x);
            direction.y = std::sin(cam.rot.z) * std::cos(cam.rot.x);
            direction.z = std::sin(cam.rot.x);
            cam.dir = glm::normalize(direction);
        }

        cube_rotation += dt * 180.f;
    };

    auto render_fn = [&]() {
        window.clear();

        auto camera_matrix = cam.matrix();

        // place the cube at (3, 3, 1) and rotate it
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, {3, 3, 1});
        model = glm::rotate(model, glm::radians(cube_rotation), glm::vec3(0,0,1));

        shader.bind();
        shader.setUniform("camera", camera_matrix);
        shader.setUniform("model", model);
        cube.bind();
        cube.render();

        shader.bind();
        shader.setUniform("camera", camera_matrix);
        shader.setUniform("model", glm::identity<glm::mat4>());
        axis.bind();
        axis.render();

        dt = static_cast<float>(window.display()) / 1000.f;
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
