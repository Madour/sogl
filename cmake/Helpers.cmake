include(FetchContent)

function(set_cache_options)
    set(flags_keywords "")
    set(one_value_keywords "")
    set(multi_value_keywords OPTIONS)
    cmake_parse_arguments(PARSE_ARGV 0 arg "${flags_keywords}" "${one_value_keywords}" "${multi_value_keywords}")

    set(options_list ${arg_OPTIONS})
    if (options_list)
        list(LENGTH options_list options_list_len)
        math(EXPR missing_option "${options_list_len} % 2")

        if (${missing_option} EQUAL 1)
            message(FATAL_ERROR "[sogl] Function set_cache_options was called with incorrect number of OPTIONS.")
        else()
            math(EXPR options_count "(${options_list_len} / 2) - 1")
            foreach(i RANGE ${options_count})
                math(EXPR i_option_name "${i} * 2")
                list(GET options_list ${i_option_name} option_name)

                math(EXPR i_option_value "${i} * 2 + 1")
                list(GET options_list ${i_option_value} option_value)

                set(${option_name} ${option_value} CACHE INTERNAL "")
            endforeach()
        endif()
    endif()
endfunction()

function(setup_dependency name)
    set(flags_keywords "")
    set(one_value_keywords VERSION URL)
    set(multi_value_keywords OPTIONS SOURCE_SUBDIR)
    cmake_parse_arguments(PARSE_ARGV 1 arg "${flags_keywords}" "${one_value_keywords}" "${multi_value_keywords}")

    message(STATUS "[sogl] Fetching ${name}...")

    set(source_subdir ".")
    if (arg_SOURCE_SUBDIR)
        set(source_subdir ${arg_SOURCE_SUBDIR})
    endif()

    set(source_dir ${PROJECT_SOURCE_DIR}/_deps/${name}-${arg_VERSION})
    set(binary_dir ${FETCHCONTENT_BASE_DIR}/${name}-${arg_VERSION}-build)
    set(subbuild_dir ${FETCHCONTENT_BASE_DIR}/${name}-${arg_VERSION}-subbuild)

    FetchContent_Declare(${name}
        URL ${arg_URL}
        SUBBUILD_DIR ${subbuild_dir}
        SOURCE_DIR ${source_dir}
        SOURCE_SUBDIR ${source_subdir}
        BINARY_DIR ${binary_dir}
    )

    set_cache_options(OPTIONS ${arg_OPTIONS})

    if (NOT EXISTS ${source_dir}/${source_subdir}/CMakeLists.txt)
        FetchContent_MakeAvailable(${name})
    else()
        add_subdirectory(${source_dir}/${source_subdir} ${binary_dir})
        FetchContent_SetPopulated(${name}
            SOURCE_DIR ${source_dir}
            BINARY_DIR ${binary_dir}
        )
    endif()

    FetchContent_GetProperties(${name})

    message(STATUS "[sogl] ${name} is configured.")

    return(PROPAGATE ${name}_SOURCE_DIR ${name}_BINARY_DIR ${name}_POPULATED)
endfunction()
