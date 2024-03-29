// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <sogl/Event.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <chrono>
#include <list>
#include <string>

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
        [[nodiscard]] auto isKeyPressed(Key key) const -> bool;

        auto getHandle() -> void*;

        void clear(const glm::vec<3, float>& color = {0, 0, 0});
        auto display() -> unsigned;

    private:
        void* m_handle = nullptr;
        glm::vec<2, int> m_size;
        std::list<Event> m_events;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_frame_time;
    };

}
