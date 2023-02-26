include(FetchContent)

function(declare_dependency name)
    set(flags_keywords "")
    set(one_value_keywords URL)
    set(multi_value_keywords "")
    cmake_parse_arguments(PARSE_ARGV 1 ${name} "${flags_keywords}" "${one_value_keywords}" "${multi_value_keywords}")

    FetchContent_Declare(
            ${name}
            URL ${${name}_URL}
            SOURCE_DIR ${PROJECT_BINARY_DIR}/_deps/${name}/src
            BINARY_DIR ${PROJECT_BINARY_DIR}/_deps/${name}/build
            SUBBUILD_DIR ${PROJECT_BINARY_DIR}/_deps/${name}/subbuild
    )
endfunction()

function(configure_dependency name)
    set(flags_keywords "")
    set(one_value_keywords "")
    set(multi_value_keywords OPTIONS)
    cmake_parse_arguments(PARSE_ARGV 1 ${name} "${flags_keywords}" "${one_value_keywords}" "${multi_value_keywords}")

    FetchContent_GetProperties(${name})

    if (NOT ${name}_POPULATED)
        FetchContent_Populate(${name})

        set(options_list ${${name}_OPTIONS})
        if (options_list)
            list(LENGTH options_list options_list_len)
            math(EXPR missing_option "${options_list_len} % 2")

            if (${missing_option} EQUAL 1)
                message(FATAL_ERROR "Function configure_dependency called with incorrect number of OPTIONS.")
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

        add_subdirectory(${${name}_SOURCE_DIR} ${${name}_BINARY_DIR})
    endif()
endfunction()

function(setup_dependency name)
    set(flags_keywords "")
    set(one_value_keywords URL)
    set(multi_value_keywords OPTIONS)
    cmake_parse_arguments(PARSE_ARGV 1 ${name} "${flags_keywords}" "${one_value_keywords}" "${multi_value_keywords}")

    message(STATUS "[sogl] Fetching ${name}...")

    declare_dependency(${name} URL ${${name}_URL})
    configure_dependency(${name} OPTIONS ${${name}_OPTIONS})

    message(STATUS "[sogl] ${name} is configured.")
endfunction()
