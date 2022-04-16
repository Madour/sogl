// Created by Modar Nasser on 16/04/2022.

#include <sogl/Inputs.hpp>

using namespace sogl;

namespace {
    auto keyToString(Key key) -> const char*;
    auto mouseButtonToString(MouseButton btn) -> const char*;
}

auto sogl::operator<<(std::ostream& os, Key key) -> std::ostream& {
    os << "Key<" << keyToString(key) << ">";
    return os;
}

auto sogl::operator<<(std::ostream& os, MouseButton btn) -> std::ostream& {
    os << "MouseButton<" << mouseButtonToString(btn) << ">";
    return os;
}

namespace {
    auto keyToString(Key key) -> const char* {
        switch (key) {
            case Key::Space:
                return "Space";
            case Key::Apostrophe:
                return "Apostrophe";
            case Key::Comma:
                return "Comma";
            case Key::Minus:
                return "Minus";
            case Key::Period:
                return "Period";
            case Key::Slash:
                return "Slash";
            case Key::Num0:
                return "Num0";
            case Key::Num1:
                return "Num1";
            case Key::Num2:
                return "Num2";
            case Key::Num3:
                return "Num3";
            case Key::Num4:
                return "Num4";
            case Key::Num5:
                return "Num5";
            case Key::Num6:
                return "Num6";
            case Key::Num7:
                return "Num7";
            case Key::Num8:
                return "Num8";
            case Key::Num9:
                return "Num9";
            case Key::Semicolon:
                return "Semicolon";
            case Key::Equal:
                return "Equal";
            case Key::A:
                return "A";
            case Key::B:
                return "B";
            case Key::C:
                return "C";
            case Key::D:
                return "D";
            case Key::E:
                return "E";
            case Key::F:
                return "F";
            case Key::G:
                return "G";
            case Key::H:
                return "H";
            case Key::I:
                return "I";
            case Key::J:
                return "J";
            case Key::K:
                return "K";
            case Key::L:
                return "L";
            case Key::M:
                return "M";
            case Key::N:
                return "N";
            case Key::O:
                return "O";
            case Key::P:
                return "P";
            case Key::Q:
                return "Q";
            case Key::R:
                return "R";
            case Key::S:
                return "S";
            case Key::T:
                return "T";
            case Key::U:
                return "U";
            case Key::V:
                return "V";
            case Key::W:
                return "W";
            case Key::X:
                return "X";
            case Key::Y:
                return "Y";
            case Key::Z:
                return "Z";
            case Key::LeftBracket:
                return "LeftBracket";
            case Key::Backslash:
                return "Backslash";
            case Key::RightBracket:
                return "RightBracket";
            case Key::GraveAccent:
                return "GraveAccent";
            case Key::Escape:
                return "Escape";
            case Key::Enter:
                return "Enter";
            case Key::Tab:
                return "Tab";
            case Key::Backspace:
                return "Backspace";
            case Key::Insert:
                return "Insert";
            case Key::Delete:
                return "Delete";
            case Key::Right:
                return "Right";
            case Key::Left:
                return "Left";
            case Key::Down:
                return "Down";
            case Key::Up:
                return "Up";
            case Key::PageUp:
                return "PageUp";
            case Key::PageDown:
                return "PageDown";
            case Key::Home:
                return "Home";
            case Key::End:
                return "End";
            case Key::CapsLock:
                return "CapsLock";
            case Key::ScrollLock:
                return "ScrollLock";
            case Key::NumLock:
                return "NumLock";
            case Key::PrintScreen:
                return "PrintScreen";
            case Key::Pause:
                return "Pause";
            case Key::F1:
                return "F1";
            case Key::F2:
                return "F2";
            case Key::F3:
                return "F3";
            case Key::F4:
                return "F4";
            case Key::F5:
                return "F5";
            case Key::F6:
                return "F6";
            case Key::F7:
                return "F7";
            case Key::F8:
                return "F8";
            case Key::F9:
                return "F9";
            case Key::F10:
                return "F10";
            case Key::F11:
                return "F11";
            case Key::F12:
                return "F12";
            case Key::F13:
                return "F13";
            case Key::F14:
                return "F14";
            case Key::F15:
                return "F15";
            case Key::F16:
                return "F16";
            case Key::F17:
                return "F17";
            case Key::F18:
                return "F18";
            case Key::F19:
                return "F19";
            case Key::F20:
                return "F20";
            case Key::F21:
                return "F21";
            case Key::F22:
                return "F22";
            case Key::F23:
                return "F23";
            case Key::F24:
                return "F24";
            case Key::F25:
                return "F25";
            case Key::Numpad0:
                return "Numpad0";
            case Key::Numpad1:
                return "Numpad1";
            case Key::Numpad2:
                return "Numpad2";
            case Key::Numpad3:
                return "Numpad3";
            case Key::Numpad4:
                return "Numpad4";
            case Key::Numpad5:
                return "Numpad5";
            case Key::Numpad6:
                return "Numpad6";
            case Key::Numpad7:
                return "Numpad7";
            case Key::Numpad8:
                return "Numpad8";
            case Key::Numpad9:
                return "Numpad9";
            case Key::NumpadPoint:
                return "NumpadPoint";
            case Key::NumpadDivide:
                return "NumpadDivide";
            case Key::NumpadMultiply:
                return "NumpadMultiply";
            case Key::NumpadSubstract:
                return "NumpadSubstract";
            case Key::NumpadAdd:
                return "NumpadAdd";
            case Key::NumpadEnter:
                return "NumpadEnter";
            case Key::NumpadEqual:
                return "NumpadEqual";
            case Key::LeftShift:
                return "LeftShift";
            case Key::LeftControl:
                return "LeftControl";
            case Key::LeftAlt:
                return "LeftAlt";
            case Key::LeftSuper:
                return "LeftSuper";
            case Key::RightShift:
                return "RightShift";
            case Key::RightControl:
                return "RightControl";
            case Key::RightAlt:
                return "RightAlt";
            case Key::RightSuper:
                return "RightSuper";
            default:
                return "Unknown";
        }
    }

    auto mouseButtonToString(MouseButton btn) -> const char* {
        switch (btn) {
            case MouseButton::Left:
                return "Left";
            case MouseButton::Middle:
                return "Middle";
            case MouseButton::Right:
                return "Right";
            case MouseButton::Extra1:
                return "Extra1";
            case MouseButton::Extra2:
                return "Extra2";
            case MouseButton::Extra3:
                return "Extra3";
            case MouseButton::Extra4:
                return "Extra4";
            case MouseButton::Extra5:
                return "Extra5";
            default:
                return "Unknown";
        }
    }
}