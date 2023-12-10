cmake_minimum_required(VERSION 3.16)

project(
    sogl
    VERSION 0.3.0
    DESCRIPTION "A simple and light library for creating OpenGL application"
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/Madour/sogl"
)

include(cmake/Helpers.cmake)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(SOGL_WASM TRUE)
else()
    set(SOGL_DESKTOP TRUE)
endif()

message(STATUS "[sogl] Configuring dependencies")
if (SOGL_DESKTOP)
    setup_dependency(glew
        URL https://github.com/Perlmint/glew-cmake/archive/refs/tags/glew-cmake-2.2.0.zip
        OPTIONS
            glew-cmake_BUILD_SHARED OFF
            glew-cmake_BUILD_STATIC ON
            PKG_CONFIG_REPRESENTATIVE_TARGET OFF
            ONLY_LIBS ON
    )
    setup_dependency(glfw
        URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip
        OPTIONS
            GLFW_BUILD_EXAMPLES OFF
            GLFW_BUILD_TESTS OFF
            GLFW_BUILD_DOCS OFF
            GLFW_INSTALL OFF
    )
endif()
setup_dependency(glm
    URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
)
setup_dependency(freetype
    URL https://download.savannah.gnu.org/releases/freetype/freetype-2.12.1.tar.xz
    OPTIONS
        FT_DISABLE_HARFBUZZ ON
)
setup_dependency(harfbuzz
    URL https://github.com/harfbuzz/harfbuzz/releases/download/7.1.0/harfbuzz-7.1.0.tar.xz
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

target_link_libraries(sogl PUBLIC glm::glm)
if(SOGL_DESKTOP)
    target_link_libraries(sogl PUBLIC libglew_static glfw freetype harfbuzz)
endif()

set_target_properties(sogl PROPERTIES DEBUG_POSTFIX -d)
set_target_properties(sogl PROPERTIES CXX_STANDARD 20)
if (SOGL_WASM)
    set_target_properties(sogl PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(examples)
endif()
