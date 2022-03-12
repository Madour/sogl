macro(add_example name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE sogl)
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX -d)
endmacro()

add_example(triangle triangle.cpp)
