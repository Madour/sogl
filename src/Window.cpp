// Created by Modar Nasser on 05/03/2022.

#include <sogl/Window.hpp>

#include <chrono>
#include <iostream>

using namespace sogl;

namespace {
    void error_callback(int error, const char* description) {
        std::cerr << "Error " << error << ": " << description << std::endl;
    }

    void GLAPIENTRY gl_debug_msg_cb(GLenum source, GLenum type, GLuint id, GLenum severity,
                                    GLsizei length, const GLchar* message, const void* param) {
        std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
                  << "id = " << id << ", source = " << source << ", type = " << type << ", severity = " << severity
                  << "\n" << message << "\n" << std::endl;
    }
    auto keyToGlfw(Key key) -> int;
}

int Window::instance_count = 0;

Window::Window(int width, int height, const std::string& title) {
    if (instance_count == 0) {
        glfwSetErrorCallback(error_callback);
        glfwInit();
    }
    m_frame_time = std::chrono::high_resolution_clock::now();
    m_size = {width, height};
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
#if !defined(EMSCRIPTEN)
    glfwSetInputMode(m_window, GLFW_LOCK_KEY_MODS, true);
#endif
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, true);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, true);

    static Window* self;
    self = this;

    auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS)
            self->m_events.push_back(Event(Event::KeyPress{}, {key, scancode, action, mods}));
        else if (action == GLFW_RELEASE)
            self->m_events.push_back(Event(Event::KeyRelease{}, {key, scancode, action, mods}));
    };

    auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS)
            self->m_events.push_back(Event(Event::MousePress{}, {button, action, mods}));
        else if (action == GLFW_RELEASE)
            self->m_events.push_back(Event(Event::MouseRelease{}, {button, action, mods}));
    };

    auto mouse_move_callback = [](GLFWwindow* window, double x, double y) {
        self->m_events.push_back(Event(Event::MouseMove{}, {static_cast<int>(x), static_cast<int>(y)}));
    };

    auto mouse_wheel_callback = [](GLFWwindow* window, double x, double y) {
        self->m_events.push_back(Event(Event::Scroll{}, {static_cast<int>(x), static_cast<int>(y)}));
    };

    auto window_size_callback = [](GLFWwindow* window, int width, int height) {
        self->m_events.push_back(Event(Event::Resize{}, {width, height}));
        self->m_size = {width, height};
        glViewport(0, 0, width, height);
    };

    auto window_drop_callback = [](GLFWwindow* window, int count, const char** paths) {
         self->m_events.push_back(Event(Event::Drop{}, {count}, paths));
    };

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPosCallback(m_window, mouse_move_callback);
    glfwSetScrollCallback(m_window, mouse_wheel_callback);
    glfwSetWindowSizeCallback(m_window, window_size_callback);
    glfwSetDropCallback(m_window, window_drop_callback);

    if (instance_count == 0) {
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize glew" << std::endl;
        }
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#if !defined(EMSCRIPTEN)
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_debug_msg_cb, nullptr);
#endif
        window_size_callback(m_window, m_size.x, m_size.y);
        std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
    }
    instance_count += 1;
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    instance_count -= 1;
    if (instance_count == 0) {
        glfwTerminate();
    }
}

auto Window::getSize() const -> const glm::vec<2, int>& {
    return m_size;
}

auto Window::isOpen() const -> bool {
    return !glfwWindowShouldClose(m_window);
}

void Window::close() {
    glfwSetWindowShouldClose(m_window, true);
}

auto Window::nextEvent() -> std::optional<Event> {
    if (m_events.empty()) {
        return std::nullopt;
    } else {
        auto ev = std::optional(m_events.back());
        m_events.pop_back();
        return ev;
    }
}

auto Window::getMousePosition() const -> glm::vec<2, int> {
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return {static_cast<int>(x), static_cast<int>(y)};
}

auto Window::isKeyPressed(Key key) const -> bool {
    return glfwGetKey(m_window, keyToGlfw(key));
}

auto Window::operator&() -> GLFWwindow* {
    return m_window;
}

void Window::clear(const glm::vec<3, float>& color) {
    glClearColor(color.r, color.g, color.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

auto Window::display() -> unsigned {
    glfwSwapBuffers(m_window);
    m_events.clear();
    glfwPollEvents();

    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_frame_time);
    m_frame_time = now;
    return duration.count();
}

namespace {
    auto keyToGlfw(Key key) -> int {
        switch (key) {
            case Key::Unknown:
                return GLFW_KEY_UNKNOWN;
            case Key::Space:
                return GLFW_KEY_SPACE;
            case Key::Apostrophe:
                return GLFW_KEY_APOSTROPHE;
            case Key::Comma:
                return GLFW_KEY_COMMA;
            case Key::Minus:
                return GLFW_KEY_MINUS;
            case Key::Period:
                return GLFW_KEY_PERIOD;
            case Key::Slash:
                return GLFW_KEY_SLASH;
            case Key::Num0:
                return GLFW_KEY_0;
            case Key::Num1:
                return GLFW_KEY_1;
            case Key::Num2:
                return GLFW_KEY_2;
            case Key::Num3:
                return GLFW_KEY_3;
            case Key::Num4:
                return GLFW_KEY_4;
            case Key::Num5:
                return GLFW_KEY_5;
            case Key::Num6:
                return GLFW_KEY_6;
            case Key::Num7:
                return GLFW_KEY_7;
            case Key::Num8:
                return GLFW_KEY_8;
            case Key::Num9:
                return GLFW_KEY_9;
            case Key::Semicolon:
                return GLFW_KEY_SEMICOLON;
            case Key::Equal:
                return GLFW_KEY_EQUAL;
            case Key::A:
                return GLFW_KEY_A;
            case Key::B:
                return GLFW_KEY_B;
            case Key::C:
                return GLFW_KEY_C;
            case Key::D:
                return GLFW_KEY_D;
            case Key::E:
                return GLFW_KEY_E;
            case Key::F:
                return GLFW_KEY_F;
            case Key::G:
                return GLFW_KEY_G;
            case Key::H:
                return GLFW_KEY_H;
            case Key::I:
                return GLFW_KEY_I;
            case Key::J:
                return GLFW_KEY_J;
            case Key::K:
                return GLFW_KEY_K;
            case Key::L:
                return GLFW_KEY_L;
            case Key::M:
                return GLFW_KEY_M;
            case Key::N:
                return GLFW_KEY_N;
            case Key::O:
                return GLFW_KEY_O;
            case Key::P:
                return GLFW_KEY_P;
            case Key::Q:
                return GLFW_KEY_Q;
            case Key::R:
                return GLFW_KEY_R;
            case Key::S:
                return GLFW_KEY_S;
            case Key::T:
                return GLFW_KEY_T;
            case Key::U:
                return GLFW_KEY_U;
            case Key::V:
                return GLFW_KEY_V;
            case Key::W:
                return GLFW_KEY_W;
            case Key::X:
                return GLFW_KEY_X;
            case Key::Y:
                return GLFW_KEY_Y;
            case Key::Z:
                return GLFW_KEY_Z;
            case Key::LeftBracket:
                return GLFW_KEY_LEFT_BRACKET;
            case Key::Backslash:
                return GLFW_KEY_BACKSLASH;
            case Key::RightBracket:
                return GLFW_KEY_RIGHT_BRACKET;
            case Key::GraveAccent:
                return GLFW_KEY_GRAVE_ACCENT;
            case Key::Escape:
                return GLFW_KEY_ESCAPE;
            case Key::Enter:
                return GLFW_KEY_ENTER;
            case Key::Tab:
                return GLFW_KEY_TAB;
            case Key::Backspace:
                return GLFW_KEY_BACKSPACE;
            case Key::Insert:
                return GLFW_KEY_INSERT;
            case Key::Delete:
                return GLFW_KEY_DELETE;
            case Key::Right:
                return GLFW_KEY_RIGHT;
            case Key::Left:
                return GLFW_KEY_LEFT;
            case Key::Down:
                return GLFW_KEY_DOWN;
            case Key::Up:
                return GLFW_KEY_UP;
            case Key::PageUp:
                return GLFW_KEY_PAGE_UP;
            case Key::PageDown:
                return GLFW_KEY_PAGE_DOWN;
            case Key::Home:
                return GLFW_KEY_HOME;
            case Key::End:
                return GLFW_KEY_END;
            case Key::CapsLock:
                return GLFW_KEY_CAPS_LOCK;
            case Key::ScrollLock:
                return GLFW_KEY_SCROLL_LOCK;
            case Key::NumLock:
                return GLFW_KEY_NUM_LOCK;
            case Key::PrintScreen:
                return GLFW_KEY_PRINT_SCREEN;
            case Key::Pause:
                return GLFW_KEY_PAUSE;
            case Key::F1:
                return GLFW_KEY_F1;
            case Key::F2:
                return GLFW_KEY_F2;
            case Key::F3:
                return GLFW_KEY_F3;
            case Key::F4:
                return GLFW_KEY_F4;
            case Key::F5:
                return GLFW_KEY_F5;
            case Key::F6:
                return GLFW_KEY_F6;
            case Key::F7:
                return GLFW_KEY_F7;
            case Key::F8:
                return GLFW_KEY_F8;
            case Key::F9:
                return GLFW_KEY_F9;
            case Key::F10:
                return GLFW_KEY_F10;
            case Key::F11:
                return GLFW_KEY_F11;
            case Key::F12:
                return GLFW_KEY_F12;
            case Key::F13:
                return GLFW_KEY_F13;
            case Key::F14:
                return GLFW_KEY_F14;
            case Key::F15:
                return GLFW_KEY_F15;
            case Key::F16:
                return GLFW_KEY_F16;
            case Key::F17:
                return GLFW_KEY_F17;
            case Key::F18:
                return GLFW_KEY_F18;
            case Key::F19:
                return GLFW_KEY_F19;
            case Key::F20:
                return GLFW_KEY_F20;
            case Key::F21:
                return GLFW_KEY_F21;
            case Key::F22:
                return GLFW_KEY_F22;
            case Key::F23:
                return GLFW_KEY_F23;
            case Key::F24:
                return GLFW_KEY_F24;
            case Key::F25:
                return GLFW_KEY_F25;
            case Key::Numpad0:
                return GLFW_KEY_KP_0;
            case Key::Numpad1:
                return GLFW_KEY_KP_1;
            case Key::Numpad2:
                return GLFW_KEY_KP_2;
            case Key::Numpad3:
                return GLFW_KEY_KP_3;
            case Key::Numpad4:
                return GLFW_KEY_KP_4;
            case Key::Numpad5:
                return GLFW_KEY_KP_5;
            case Key::Numpad6:
                return GLFW_KEY_KP_6;
            case Key::Numpad7:
                return GLFW_KEY_KP_7;
            case Key::Numpad8:
                return GLFW_KEY_KP_8;
            case Key::Numpad9:
                return GLFW_KEY_KP_9;
            case Key::NumpadPoint:
                return GLFW_KEY_KP_DECIMAL;
            case Key::NumpadDivide:
                return GLFW_KEY_KP_DIVIDE;
            case Key::NumpadMultiply:
                return GLFW_KEY_KP_MULTIPLY;
            case Key::NumpadSubstract:
                return GLFW_KEY_KP_SUBTRACT;
            case Key::NumpadAdd:
                return GLFW_KEY_KP_ADD;
            case Key::NumpadEnter:
                return GLFW_KEY_KP_ENTER;
            case Key::NumpadEqual:
                return GLFW_KEY_KP_EQUAL;
            case Key::LeftShift:
                return GLFW_KEY_LEFT_SHIFT;
            case Key::LeftControl:
                return GLFW_KEY_LEFT_CONTROL;
            case Key::LeftAlt:
                return GLFW_KEY_LEFT_ALT;
            case Key::LeftSuper:
                return GLFW_KEY_LEFT_SUPER;
            case Key::RightShift:
                return GLFW_KEY_RIGHT_SHIFT;
            case Key::RightControl:
                return GLFW_KEY_RIGHT_CONTROL;
            case Key::RightAlt:
                return GLFW_KEY_RIGHT_ALT;
            case Key::RightSuper:
                return GLFW_KEY_RIGHT_SUPER;
            default:
                return GLFW_KEY_UNKNOWN;
        }
    }
}