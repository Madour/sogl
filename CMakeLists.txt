cmake_minimum_required(VERSION 3.26)

include(cmake/Policies.cmake)
configure_policies()

project(
    sogl
    VERSION 0.4.0
    DESCRIPTION "A simple and light library for creating OpenGL application"
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/Madour/sogl"
)

include(cmake/Helpers.cmake)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(SOGL_WASM TRUE)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    set(SOGL_DESKTOP TRUE)
endif()

set(BUILD_SHARED_LIBS OFF)

message(STATUS "[sogl] Configuring dependencies")
if (SOGL_DESKTOP)
    setup_dependency(glew VERSION 2.3.1
        URL https://github.com/Perlmint/glew-cmake/archive/refs/tags/glew-cmake-2.3.1.zip
        OPTIONS
            glew-cmake_BUILD_SHARED OFF
            glew-cmake_BUILD_STATIC ON
            PKG_CONFIG_REPRESENTATIVE_TARGET OFF
            ONLY_LIBS ON
    )
    setup_dependency(glfw VERSION 3.3.8
        URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip
        OPTIONS
            GLFW_BUILD_EXAMPLES OFF
            GLFW_BUILD_TESTS OFF
            GLFW_BUILD_DOCS OFF
            GLFW_INSTALL OFF
    )
endif()
setup_dependency(glm VERSION 1.0.3
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
)
setup_dependency(freetype VERSION 2.14.1
    URL https://download-mirror.savannah.gnu.org/releases/freetype/freetype-2.14.1.tar.xz
    OPTIONS
        FT_DISABLE_HARFBUZZ ON
)
setup_dependency(harfbuzz VERSION 12.3.2
    URL https://github.com/harfbuzz/harfbuzz/releases/download/12.3.2/harfbuzz-12.3.2.tar.xz
    OPTIONS
        HB_BUILD_SUBSET OFF
        HB_HAVE_INTROSPECTION OFF
)

set(SOGL_SOURCES
    src/thirdparty/stb_image.h
    src/thirdparty/stb_image_write.h
    src/Event.cpp
    src/Font.cpp
    src/Framebuffer.cpp
    src/Inputs.cpp
    src/PostProcess.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/Window.cpp
)
set(SOGL_HEADERS
    include/sogl/Event.hpp
    include/sogl/Font.hpp
    include/sogl/Framebuffer.hpp
    include/sogl/Inputs.hpp
    include/sogl/PostProcess.hpp
    include/sogl/Shader.hpp
    include/sogl/Shader.tpp
    include/sogl/sogl.hpp
    include/sogl/Texture.hpp
    include/sogl/Vertex.hpp
    include/sogl/VertexArray.hpp
    include/sogl/VertexArray.tpp
    include/sogl/Window.hpp
)
add_library(sogl STATIC ${SOGL_SOURCES} ${SOGL_HEADERS})
add_library(sogl::sogl ALIAS sogl)
target_include_directories(sogl PUBLIC include)

target_link_libraries(sogl PUBLIC glm::glm freetype harfbuzz)
if(SOGL_DESKTOP)
    target_link_libraries(sogl PUBLIC libglew_static glfw)
endif()

set_target_properties(sogl PROPERTIES
    DEBUG_POSTFIX -d
    CXX_STANDARD 20
)
if (SOGL_WASM)
    target_link_options(sogl PUBLIC -sUSE_GLFW=3 -sUSE_WEBGL2=1 -sFULL_ES3=1)
endif()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(examples)
endif()
