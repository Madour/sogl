// Created by Modar Nasser on 05/03/2022.

#include <sogl/Event.hpp>

#include <sogl/Inputs.hpp>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#endif
#include <GLFW/glfw3.h>

#include <utility>

using namespace sogl;

namespace {
    auto keyFromGlfw(int key) -> Key;
}

Event::Event(EventTypes data) : m_data(std::move(data))
{}

Event::Event(const EventTypes& event, std::initializer_list<int> args, const char** paths) {
    if (std::holds_alternative<KeyPress>(event)) {
        auto key = *(args.begin() + 0);
        auto scancode = *(args.begin() + 1);
        auto action = *(args.begin() + 2);
        auto mods = *(args.begin() + 3);
        m_data = KeyPress{keyFromGlfw(key), scancode, mods};
    }
    else if (std::holds_alternative<KeyRelease>(event)) {
        auto key = *(args.begin() + 0);
        auto scancode = *(args.begin() + 1);
        auto action = *(args.begin() + 2);
        auto mods = *(args.begin() + 3);
        m_data = KeyRelease{keyFromGlfw(key), scancode, mods};
    }
    else if (std::holds_alternative<MousePress>(event)) {
        auto button = *(args.begin() + 0);
        auto action = *(args.begin() + 1);
        auto mods = *(args.begin() + 2);
        m_data = Event::MousePress{button, mods};
    }
    else if (std::holds_alternative<MouseRelease>(event)) {
        auto button = *(args.begin() + 0);
        auto action = *(args.begin() + 1);
        auto mods = *(args.begin() + 2);
        m_data = Event::MouseRelease{button, mods};
    }
    else if (std::holds_alternative<MouseMove>(event)) {
        auto x = *(args.begin() + 0);
        auto y = *(args.begin() + 1);
        m_data = Event::MouseMove{x, y};
    }
    else if (std::holds_alternative<Scroll>(event)) {
        auto dx = *(args.begin() + 0);
        auto dy = *(args.begin() + 1);
        m_data = Event::Scroll{dx, dy};
    }
    else if (std::holds_alternative<Resize>(event)) {
        auto width = *(args.begin() + 0);
        auto height = *(args.begin() + 1);
        m_data = Event::Resize{width, height};
    }
    else if (std::holds_alternative<Drop>(event)) {
        auto count = *(args.begin() + 0);
        Event::Drop drop;
        drop.files.resize(count);
        for (int i = 0; i < count; ++i)
            drop.files[i] = paths[i];
        m_data = drop;
    }
}

namespace {
    auto keyFromGlfw(int key) -> Key {
        switch (key) {
            case GLFW_KEY_UNKNOWN:
                return Key::Unknown;
            case GLFW_KEY_SPACE:
                return Key::Space;
            case GLFW_KEY_APOSTROPHE:
                return Key::Apostrophe;
            case GLFW_KEY_COMMA:
                return Key::Comma;
            case GLFW_KEY_MINUS:
                return Key::Minus;
            case GLFW_KEY_PERIOD:
                return Key::Period;
            case GLFW_KEY_SLASH:
                return Key::Slash;
            case GLFW_KEY_0:
                return Key::Num0;
            case GLFW_KEY_1:
                return Key::Num1;
            case GLFW_KEY_2:
                return Key::Num2;
            case GLFW_KEY_3:
                return Key::Num3;
            case GLFW_KEY_4:
                return Key::Num4;
            case GLFW_KEY_5:
                return Key::Num5;
            case GLFW_KEY_6:
                return Key::Num6;
            case GLFW_KEY_7:
                return Key::Num7;
            case GLFW_KEY_8:
                return Key::Num8;
            case GLFW_KEY_9:
                return Key::Num9;
            case GLFW_KEY_SEMICOLON:
                return Key::Semicolon;
            case GLFW_KEY_EQUAL:
                return Key::Equal;
            case GLFW_KEY_A:
                return Key::A;
            case GLFW_KEY_B:
                return Key::B;
            case GLFW_KEY_C:
                return Key::C;
            case GLFW_KEY_D:
                return Key::D;
            case GLFW_KEY_E:
                return Key::E;
            case GLFW_KEY_F:
                return Key::F;
            case GLFW_KEY_G:
                return Key::G;
            case GLFW_KEY_H:
                return Key::H;
            case GLFW_KEY_I:
                return Key::I;
            case GLFW_KEY_J:
                return Key::J;
            case GLFW_KEY_K:
                return Key::K;
            case GLFW_KEY_L:
                return Key::L;
            case GLFW_KEY_M:
                return Key::M;
            case GLFW_KEY_N:
                return Key::N;
            case GLFW_KEY_O:
                return Key::O;
            case GLFW_KEY_P:
                return Key::P;
            case GLFW_KEY_Q:
                return Key::Q;
            case GLFW_KEY_R:
                return Key::R;
            case GLFW_KEY_S:
                return Key::S;
            case GLFW_KEY_T:
                return Key::T;
            case GLFW_KEY_U:
                return Key::U;
            case GLFW_KEY_V:
                return Key::V;
            case GLFW_KEY_W:
                return Key::W;
            case GLFW_KEY_X:
                return Key::X;
            case GLFW_KEY_Y:
                return Key::Y;
            case GLFW_KEY_Z:
                return Key::Z;
            case GLFW_KEY_LEFT_BRACKET:
                return Key::LeftBracket;
            case GLFW_KEY_BACKSLASH:
                return Key::Backslash;
            case GLFW_KEY_RIGHT_BRACKET:
                return Key::RightBracket;
            case GLFW_KEY_GRAVE_ACCENT:
                return Key::GraveAccent;
            case GLFW_KEY_WORLD_1:
                return Key::Unknown;
            case GLFW_KEY_WORLD_2:
                return Key::Unknown;
            case GLFW_KEY_ESCAPE:
                return Key::Escape;
            case GLFW_KEY_ENTER:
                return Key::Enter;
            case GLFW_KEY_TAB:
                return Key::Tab;
            case GLFW_KEY_BACKSPACE:
                return Key::Backspace;
            case GLFW_KEY_INSERT:
                return Key::Insert;
            case GLFW_KEY_DELETE:
                return Key::Delete;
            case GLFW_KEY_RIGHT:
                return Key::Right;
            case GLFW_KEY_LEFT:
                return Key::Left;
            case GLFW_KEY_DOWN:
                return Key::Down;
            case GLFW_KEY_UP:
                return Key::Up;
            case GLFW_KEY_PAGE_UP:
                return Key::PageUp;
            case GLFW_KEY_PAGE_DOWN:
                return Key::PageDown;
            case GLFW_KEY_HOME:
                return Key::Home;
            case GLFW_KEY_END:
                return Key::End;
            case GLFW_KEY_CAPS_LOCK:
                return Key::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:
                return Key::ScrollLock;
            case GLFW_KEY_NUM_LOCK:
                return Key::NumLock;
            case GLFW_KEY_PRINT_SCREEN:
                return Key::PrintScreen;
            case GLFW_KEY_PAUSE:
                return Key::Pause;
            case GLFW_KEY_F1:
                return Key::F1;
            case GLFW_KEY_F2:
                return Key::F2;
            case GLFW_KEY_F3:
                return Key::F3;
            case GLFW_KEY_F4:
                return Key::F4;
            case GLFW_KEY_F5:
                return Key::F5;
            case GLFW_KEY_F6:
                return Key::F6;
            case GLFW_KEY_F7:
                return Key::F7;
            case GLFW_KEY_F8:
                return Key::F8;
            case GLFW_KEY_F9:
                return Key::F9;
            case GLFW_KEY_F10:
                return Key::F10;
            case GLFW_KEY_F11:
                return Key::F11;
            case GLFW_KEY_F12:
                return Key::F12;
            case GLFW_KEY_F13:
                return Key::F13;
            case GLFW_KEY_F14:
                return Key::F14;
            case GLFW_KEY_F15:
                return Key::F15;
            case GLFW_KEY_F16:
                return Key::F16;
            case GLFW_KEY_F17:
                return Key::F17;
            case GLFW_KEY_F18:
                return Key::F18;
            case GLFW_KEY_F19:
                return Key::F19;
            case GLFW_KEY_F20:
                return Key::F20;
            case GLFW_KEY_F21:
                return Key::F21;
            case GLFW_KEY_F22:
                return Key::F22;
            case GLFW_KEY_F23:
                return Key::F23;
            case GLFW_KEY_F24:
                return Key::F24;
            case GLFW_KEY_F25:
                return Key::F25;
            case GLFW_KEY_KP_0:
                return Key::Numpad0;
            case GLFW_KEY_KP_1:
                return Key::Numpad1;
            case GLFW_KEY_KP_2:
                return Key::Numpad2;
            case GLFW_KEY_KP_3:
                return Key::Numpad3;
            case GLFW_KEY_KP_4:
                return Key::Numpad4;
            case GLFW_KEY_KP_5:
                return Key::Numpad5;
            case GLFW_KEY_KP_6:
                return Key::Numpad6;
            case GLFW_KEY_KP_7:
                return Key::Numpad7;
            case GLFW_KEY_KP_8:
                return Key::Numpad8;
            case GLFW_KEY_KP_9:
                return Key::Numpad9;
            case GLFW_KEY_KP_DECIMAL:
                return Key::NumpadPoint;
            case GLFW_KEY_KP_DIVIDE:
                return Key::NumpadDivide;
            case GLFW_KEY_KP_MULTIPLY:
                return Key::NumpadMultiply;
            case GLFW_KEY_KP_SUBTRACT:
                return Key::NumpadSubstract;
            case GLFW_KEY_KP_ADD:
                return Key::NumpadAdd;
            case GLFW_KEY_KP_ENTER:
                return Key::NumpadEnter;
            case GLFW_KEY_KP_EQUAL:
                return Key::NumpadEqual;
            case GLFW_KEY_LEFT_SHIFT:
                return Key::LeftShift;
            case GLFW_KEY_LEFT_CONTROL:
                return Key::LeftControl;
            case GLFW_KEY_LEFT_ALT:
                return Key::LeftAlt;
            case GLFW_KEY_LEFT_SUPER:
                return Key::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT:
                return Key::RightShift;
            case GLFW_KEY_RIGHT_CONTROL:
                return Key::RightControl;
            case GLFW_KEY_RIGHT_ALT:
                return Key::RightAlt;
            case GLFW_KEY_RIGHT_SUPER:
                return Key::RightSuper;
            case GLFW_KEY_MENU:
                return Key::Unknown;
            default:
                return Key::Unknown;
        }
    }
}