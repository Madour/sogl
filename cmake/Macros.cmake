include(FetchContent)

macro(setup_dependencies)
    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
        FetchContent_Declare(
                glew
                GIT_REPOSITORY https://github.com/Perlmint/glew-cmake/
                GIT_TAG glew-cmake-2.2.0
        )
        if(NOT glew_POPULATED)
            message(STATUS "[sogl] Fetching GLEW 2.2.0")
            FetchContent_Populate(glew)
            set(glew-cmake_BUILD_SHARED OFF CACHE INTERNAL "")
            set(glew-ONLY_LIBS ON CACHE INTERNAL "")
            add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR})
        endif()

        FetchContent_Declare(
                glfw
                GIT_REPOSITORY https://github.com/glfw/glfw
                GIT_TAG 3.3.6
        )
        if(NOT glfw_POPULATED)
            message(STATUS "[sogl] Fetching GLFW 3.3.6")
            FetchContent_Populate(glfw)
            set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
            set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
            set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
            set(GLFW_INSTALL OFF CACHE INTERNAL "")
            add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
        endif()
    endif()

    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm
            GIT_TAG 0.9.9.8
    )
    if(NOT glm_POPULATED)
        message(STATUS "[sogl] Fetching glm 0.9.9.8")
        FetchContent_Populate(glm)
        add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    endif()
endmacro()