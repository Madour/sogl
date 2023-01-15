# sogl

>**This is a WIP project**

sogl (Simple OpenGL) is a light library for creating OpenGL applications in C++20.

It is best used as a subproject (using either git submodules or CMake FetchContent).

The goal of this library is to provide quick and simple tools for creating OpenGL applications.

### Supported features

- Easily open a Window with an OpenGL context
- Window events (KeyPress/KeyRelease, MousePress/MouseRelease, MouseMove, Scroll, Resize, Drop)
- Textures from file
- Shaders (vertex and fragment)
- VertexArray (using vertex buffers and index buffers)

### Get started

Just download sogl and add these lines to your CMakeLists

```cmake
add_subdirectory(sogl)
target_link_libraries(YOUR_TARGET PUBLIC sogl::sogl)
```

sogl will automatically download and link its dependencies.

Here is a sample code to try out :

```c++
#include <iostream>
#include <sogl/sogl.hpp>

int main() {
    auto window = sogl::Window(960, 540, "sogl app");

    while (window.isOpen()) {
        while (auto event = window.nextEvent()) {
            if (auto press = event->as<sogl::Event::KeyPress>()) {
                if (press->key == sogl::Key::Escape) {
                    window.close();
                }
            }
            else if (auto mouse_press = event->as<sogl::Event::MousePress>()){
                if (mouse_press->button == sogl::MouseButton::Left) {
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

- [Triangle](https://github.com/Madour/sogl/blob/master/examples/01_triangle.cpp)
- [Events](https://github.com/Madour/sogl/blob/master/examples/02_events.cpp)
- [Cube](https://github.com/Madour/sogl/blob/master/examples/03_cube.cpp)

### Showcase

- [LDtkViewer](https://github.com/Madour/LDtkViewer) app uses **sogl** alongside **Dear ImGui**

![screenshot1](https://user-images.githubusercontent.com/11854124/161399326-e651cff6-383a-4d0d-9b0c-578cbdcbebf7.png)

