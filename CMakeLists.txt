cmake_minimum_required(VERSION 3.16)

project(
    sogl
    VERSION 0.3.0
    DESCRIPTION "A simple and light library for creating OpenGL application"
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/Madour/sogl"
)

include(cmake/Macros.cmake)

message(STATUS "[sogl] Configuring dependencies (glew, glfw and glm)")
fetch_dependencies()
add_subdirectory(deps)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/sogl/*.hpp include/sogl/*.tpp)

add_library(sogl STATIC ${SRC} ${INC})
target_include_directories(sogl PUBLIC include)
set_target_properties(sogl PROPERTIES DEBUG_POSTFIX -d)
set_target_properties(sogl PROPERTIES CXX_STANDARD 20)
target_link_libraries(sogl PUBLIC glm::glm)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    target_link_libraries(sogl PUBLIC libglew_static glfw)
else()
    set_target_properties(sogl PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(examples)
endif()
