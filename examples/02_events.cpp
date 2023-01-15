// Created by Modar Nasser on 15/04/2022.

#include <sogl/sogl.hpp>

#include <iostream>

void processEvent(const sogl::Event& event) {
    if (auto key_press = event.as<sogl::Event::KeyPress>()) {
        std::cout << "KeyPress: " << std::hex
                    << key_press->key << " "
                    << key_press->mods << std::endl;
    }
    else if (auto key_release = event.as<sogl::Event::KeyPress>()) {
        std::cout << "KeyRelease: " << std::hex
                    << key_release->key << " "
                    << key_release->mods << std::endl;
    }
    else if (auto mouse_press = event.as<sogl::Event::MousePress>()) {
        std::cout << "MousePress: " << std::hex
                    << mouse_press->button << " "
                    << mouse_press->mods << std::endl;
    }
    else if (auto mouse_release = event.as<sogl::Event::MouseRelease>()) {
        std::cout << "MouseRelease: " << std::hex
                    << mouse_release->button << " "
                    << mouse_release->mods << std::endl;
    }
    else if (auto mouse_move = event.as<sogl::Event::MouseMove>()) {
        std::cout << "MouseMove: " << std::dec
                    << mouse_move->x << " "
                    << mouse_move->y << std::endl;
    }
    else if (auto scroll = event.as<sogl::Event::Scroll>()) {
        std::cout << "Scroll: " << std::dec
                    << scroll->dx << " "
                    << scroll->dy << std::endl;
    }
    else if (auto resize = event.as<sogl::Event::Resize>()) {
        std::cout << "Resize: " << std::dec
                    << resize->width << " "
                    << resize->height << std::endl;
    }
    else if (auto drop = event.as<sogl::Event::Drop>()) {
        std::cout << "Drop: "
                    << drop->files.size() << " files" << std::endl;
    }
}

int main() {
    // create a window
    auto window = sogl::Window(960, 540, "Events demo");

    auto update_fn = [&]() {
        while (auto event = window.nextEvent()) {
            processEvent(event.value());
        }
    };

    auto render_fn = [&]() {
        window.clear();
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
