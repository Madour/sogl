// Created by Modar Nasser on 26/11/2023.

#include <sogl/PostProcess.hpp>


using namespace sogl;

PostProcess::PostProcess(std::string fragment_shader_src) :
    fragment_src(std::move(fragment_shader_src)) {}
