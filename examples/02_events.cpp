// Created by Modar Nasser on 15/04/2022.

#include <sogl/sogl.hpp>

#include <iostream>

int main() {
    // create a window
    auto window = sogl::Window(960, 540, "Events demo");

    // start main loop
    while (window.isOpen()) {
        while (auto event = window.nextEvent()) {
            if (auto key_press = event->as<sogl::Event::KeyPress>()) {
                std::cout << "KeyPress: " << key_press->key << " " << key_press->mods << std::endl;
            }
            else if (auto key_release = event->as<sogl::Event::KeyPress>()) {
                std::cout << "KeyRelease: " << key_release->key << " " << key_release->mods << std::endl;
            }
            else if (auto mouse_press = event->as<sogl::Event::MousePress>()) {
                std::cout << "MousePress: " << mouse_press->button << " " << mouse_press->mods << std::endl;
            }
            else if (auto mouse_release = event->as<sogl::Event::MouseRelease>()) {
                std::cout << "MouseRelease: " << mouse_release->button << " " << mouse_release->mods << std::endl;
            }
            else if (auto mouse_move = event->as<sogl::Event::MouseMove>()) {
                std::cout << "MouseMove: " << mouse_move->x << " " << mouse_move->y << std::endl;
            }
            else if (auto scroll = event->as<sogl::Event::Scroll>()) {
                std::cout << "Scroll: " << scroll->dx << " " << scroll->dy << std::endl;
            }
            else if (auto resize = event->as<sogl::Event::Resize>()) {
                std::cout << "Resize: " << resize->width << " " << resize->height << std::endl;
            }
            else if (auto drop = event->as<sogl::Event::Drop>()) {
                std::cout << "Drop: " << drop->files.size() << " files" << std::endl;
            }
        }
        window.clear();
        window.display();
    }
    return 0;
}
