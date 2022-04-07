// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <sogl/Event.hpp>

#include <GL/glew.h>
#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <optional>
#include <string>
#include <list>

namespace sogl {

    class Window {
        static int instance_count;

    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        [[nodiscard]] auto getSize() const -> const glm::vec<2, int>&;

        [[nodiscard]] auto isOpen() const -> bool;
        void close();

        auto nextEvent() -> std::optional<Event>;

        [[nodiscard]] auto getMousePosition() const -> glm::vec<2, int>;

        auto operator&() -> GLFWwindow*;

        void clear(const glm::vec<3, float>& color = {0, 0, 0});
        void display();

    private:
        GLFWwindow* m_window;
        glm::vec<2, int> m_size;
        std::list<Event> m_events;
    };

}
