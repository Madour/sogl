// Created by Modar Nasser on 26/11/2023.

#include <sogl/PostProcess.hpp>


using namespace sogl;

namespace {
    constexpr const auto* QUAD_VERT_SRC = GLSL(330 core,
        precision highp float;
        layout(location = 0) in vec2 i_pos;
        layout(location = 1) in vec2 i_uv;

        out vec2 uv;

        void main(){
            gl_Position = vec4(i_pos.xy, 0., 1.);
            uv = i_uv;
        }
    );

    constexpr const auto* QUAD_FRAG_SRC = GLSL(330 core,
        precision highp float;
        uniform sampler2D texture0;
        uniform float time;
        uniform vec2 resolution;

        in vec2 uv;
        out vec4 color;

        vec4 postprocess(vec2 uv);

        void main(){
            vec4 c = postprocess(uv);
            color = c;
        }
    );
}

PostProcess::PostProcess(const std::string& post_process_shader_src) {

    set(post_process_shader_src);
}

void PostProcess::set(const std::string& post_process_shader_src) {
    auto vert_shader = Shader::compileVertex(QUAD_VERT_SRC);
    auto frag_shader = Shader::compileFragment(QUAD_FRAG_SRC);
    auto postprocess_shader = Shader::compileFragment(post_process_shader_src);

    shader.load(vert_shader, frag_shader, postprocess_shader);

    vert_shader.destroy();
    frag_shader.destroy();
    postprocess_shader.destroy();
}

auto PostProcess::getDefault() -> const PostProcess& {
    static auto loaded = false;
    static PostProcess default_post_process;

    if (!loaded) {
        constexpr const auto* default_post_process_src = GLSL(330 core,
            uniform sampler2D texture0;
            vec4 postprocess(vec2 uv) {
                return texture(texture0, uv);
            }
        );
        default_post_process.set(default_post_process_src);
        loaded = true;
    }

    return default_post_process;
}
