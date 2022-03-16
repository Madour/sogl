// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace sogl {

    class Event {
        friend class Window;

    public:
        struct KeyPress { int key; int scancode; int mods; };
        struct KeyRelease { int key; int scancode; int mods; };
        struct MousePress { int button; int mods; };
        struct MouseRelease { int button; int mods; };
        struct MouseMove { int x; int y; };
        struct Scroll { int dx; int dy; };
        struct Resize { int width; int height; };
        struct Drop { std::vector<std::string> files; };

        Event() = default;

        template <typename T>
        constexpr auto as() -> std::optional<T> {
            if (std::holds_alternative<T>(this->m_data)) {
                return std::get<T>(this->m_data);
            }
            return std::nullopt;
        }

    private:
        using EventTypes = std::variant<KeyPress, KeyRelease,
                                        MousePress, MouseRelease,
                                        MouseMove, Scroll,
                                        Resize, Drop>;
        EventTypes m_data;

        Event(EventTypes);
    };

}