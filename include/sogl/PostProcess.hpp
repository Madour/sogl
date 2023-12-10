// Created by Modar Nasser on 26/11/2023.

#pragma once

#include <string>

namespace sogl {

    class PostProcess {
        friend class FrameBuffer;
    public:
        PostProcess(std::string fragment_shader_src);
    private:
        std::string fragment_src;
    };

} // namespace sogl
