// Created by Modar Nasser on 05/03/2022.

#include <sogl/Window.hpp>

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
}

int Window::instance_count = 0;

Window::Window(int width, int height, const std::string& title) {
    if (instance_count == 0) {
        glfwSetErrorCallback(error_callback);
        glfwInit();
    }
    m_size = {width, height};
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, true);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, true);

    static Window* self;
    self = this;

    auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        self->m_events.push_back(Event(Event::Key{key, scancode, action, mods}));
    };

    auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods) {
        self->m_events.push_back(Event(Event::MouseButton{button, action, mods}));
    };

    auto mouse_move_callback = [](GLFWwindow* window, double x, double y) {
        self->m_events.push_back(Event(Event::MouseMove{static_cast<int>(x), static_cast<int>(y)}));
    };

    auto mouse_wheel_callback = [](GLFWwindow* window, double x, double y) {
        self->m_events.push_back(Event(Event::Scroll{static_cast<int>(x), static_cast<int>(y)}));
    };

    auto window_size_callback = [](GLFWwindow* window, int width, int height) {
        self->m_events.push_back(Event(Event::Resize{width, height}));
        self->m_size = {width, height};
        glViewport(0, 0, width, height);
    };

    auto window_drop_callback = [](GLFWwindow* window, int count, const char** paths) {
        Event::Drop event;
        event.files.resize(count);
        for (int i = 0; i < count; ++i) {
            event.files[i] = paths[i];
        }
        self->m_events.push_back(Event(event));
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
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEBUG_OUTPUT); glDebugMessageCallback(gl_debug_msg_cb, nullptr);
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

void Window::display() {
    glfwSwapBuffers(m_window);
    m_events.clear();
    glfwPollEvents();
}
