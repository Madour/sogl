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

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/sogl/*.hpp include/sogl/*.tpp)

add_library(sogl STATIC ${SRC} ${INC})
add_library(sogl::sogl ALIAS sogl)
target_include_directories(sogl PUBLIC include)

target_link_libraries(sogl PUBLIC glm::glm)
if(SOGL_DESKTOP)
    target_link_libraries(sogl PUBLIC libglew_static glfw)
endif()

set_target_properties(sogl PROPERTIES DEBUG_POSTFIX -d)
set_target_properties(sogl PROPERTIES CXX_STANDARD 20)
if (SOGL_WASM)
    set_target_properties(sogl PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(examples)
endif()
