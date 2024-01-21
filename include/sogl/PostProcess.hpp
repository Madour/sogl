// Created by Modar Nasser on 26/11/2023.

#pragma once

#include <sogl/Shader.hpp>

#include <string>

namespace sogl {

    class PostProcess {
        friend class FrameBuffer;

    public:
        PostProcess(const std::string& post_process_shader_src);

        static auto getDefault() -> const PostProcess&;

    private:
        PostProcess() = default;
        void set(const std::string& post_process_shader_src);

        Shader shader;
    };

} // namespace sogl
