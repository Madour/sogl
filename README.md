# sogl

>**This is a WIP project**

sogl (for Simple OpenGL) is a light library for creating OpenGL applications.

It is best used as a subproject (either using git submodules or CMake FetchContent)

The goal of this library is to provide quick and simple tools for creating OpenGL applications.
It is not meant to become a fully fledged graphics API, just the minimum to be able to start right off the bat
and get going.

### Supported features

- Easily open a Window with an OpenGL context
- Window events (KeyPress/KeyRelease, MousePress/MouseRelease, MouseMove, Scroll, Resize, Drop)
- Textures from file
- Shaders (vertex and fragment)
- VertexArray (using vertex buffers and index buffers)

### Get started

Just download sogl and add this line to your CMakeLists

```cmake
add_subdirectory(sogl)
```

sogl will automatically download and link its dependencies.

Here is a sample code to try out :

```c++
#include <iostream>
#include <sogl/sogl.hpp>

int main() {
    auto window = sogl::Window(1000, 800, "Triangle demo");

    while (window.isOpen()) {
        while (auto event = window.nextEvent()) {
            if (auto key = event->as<sogl::Event::Key>()) {
                if (key->key == GLFW_KEY_ESCAPE) {
                    window.close();
                }
            }
            else if (auto click = event->as<sogl::Event::MouseButton>()){
                if (click->button == GLFW_MOUSE_BUTTON_LEFT && click->action == GLFW_PRESS) {
                    auto mouse_pos = window.getMousePosition();
                    std::cout << "Left click at (" << mouse_pos.x << ", " << mouse_pos.y << ")\n";
                }
            }
        }
        window.clear();
        window.display();
    }

    return 0;
}
```

### Examples

- [Triangle](https://github.com/Madour/sogl/blob/master/examples/triangle.cpp)

