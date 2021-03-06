cmake_minimum_required(VERSION 3.12)
project(sogl)

set(CMAKE_CXX_STANDARD 17)

include(cmake/Macros.cmake)

message(STATUS "Configuring dependencies (glew, glfw and glm)")
setup_dependencies()

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/sogl/*.hpp)

add_library(sogl STATIC ${SRC} ${INC})
target_include_directories(sogl PUBLIC include)
set_target_properties(sogl PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(sogl PUBLIC glm)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    target_link_libraries(sogl PUBLIC libglew_static glfw)
else()
    set_target_properties(sogl PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(examples)
endif()
