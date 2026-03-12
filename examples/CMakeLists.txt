macro(add_example name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE sogl::sogl)
    set_target_properties(${name} PROPERTIES
        DEBUG_POSTFIX -d
        CXX_STANDARD 20
    )
    if (SOGL_WASM)
       target_link_options(${name} PRIVATE -sNO_DISABLE_EXCEPTION_CATCHING)
    endif()

    set(assets_dir "")

    set(extra_args ${ARGN})
    list(LENGTH extra_args extra_args_count)
    if (${extra_args_count} GREATER 0)
        list(GET extra_args 0 assets_dir)
    endif ()

    if (NOT ${assets_dir} STREQUAL "")
        add_custom_command(PRE_BUILD TARGET ${name}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/${assets_dir} $<TARGET_FILE_DIR:${name}>/${assets_dir}
        )

        if (SOGL_WASM)
            target_link_options(${name} PRIVATE -sFORCE_FILESYSTEM --embed-file ${assets_dir}/)
        endif()
    endif()
endmacro()

add_example(01-triangle         01_triangle.cpp)
add_example(02-events           02_events.cpp)
add_example(03-cube             03_cube.cpp)
add_example(04-camera           04_camera.cpp)
add_example(05-post-processing  05_post_processing.cpp)
