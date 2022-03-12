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
target_link_libraries(sogl PUBLIC libglew_static glfw glm)
set_target_properties(sogl PROPERTIES DEBUG_POSTFIX -d)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    add_subdirectory(examples)
endif()
