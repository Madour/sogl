// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <sogl/Inputs.hpp>

#include <initializer_list>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace sogl {

    class Event {
        friend class Window;

    public:
        struct KeyPress { Key key; int scancode; KeyMod mods; };
        struct KeyRelease { Key key; int scancode; KeyMod mods; };
        struct MousePress { MouseButton button; KeyMod mods; };
        struct MouseRelease { MouseButton button; KeyMod mods; };
        struct MouseMove { int x; int y; };
        struct Scroll { int dx; int dy; };
        struct Resize { int width; int height; };
        struct Drop { std::vector<std::string> files; };

        Event() = default;

        template <typename T>
        constexpr auto as() const -> std::optional<T> {
            if (std::holds_alternative<T>(this->m_data)) {
                return std::get<T>(this->m_data);
            }
            return std::nullopt;
        }

    private:
        using EventTypes = std::variant<KeyPress, KeyRelease, MousePress, MouseRelease,
                                        MouseMove, Scroll, Resize, Drop>;
        EventTypes m_data;

        Event(EventTypes);
        Event(const EventTypes& event, std::initializer_list<int> args, const char** paths=nullptr);
    };

}
