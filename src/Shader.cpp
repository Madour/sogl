// Created by Modar Nasser on 28/02/2022.

#include <sogl/Shader.hpp>

#include <GL/glew.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>

using namespace sogl;

namespace {
    auto get_shader_info_log(unsigned shader) -> std::string {
        int length;
        std::string message;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        message.resize(length);
        glGetShaderInfoLog(shader, length, nullptr, message.data());
        return message;
    }

    auto to_string(const Shader::Type& type) -> std::string {
        switch (type) {
                case Shader::Type::Vertex: return "Vertex";
                case Shader::Type::Fragment: return "Fragment";
                case Shader::Type::Geometry: return "Geometry";
                default: return "Unknown";
            }
    }

    auto compile(Shader::Type type, const std::string& src, const unsigned& handle) -> bool {
        auto* src_c = src.c_str();
        glShaderSource(handle, 1, &src_c, nullptr);
        glCompileShader(handle);

        int status;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            std::cerr << "Failed to compile " << to_string(type) << " shader :\n"
                      << get_shader_info_log(handle) << std::endl;
            return false;
        }

        return true;
    }

    std::vector<std::string> split(const std::string& input, char delimiter)
    {
        std::vector<std::string> result;

        for (auto&& part : input | std::views::split(delimiter))
            result.emplace_back(part.begin(), part.end());

        return result;
    }

    auto compile(Shader::Type type, const std::vector<std::string>& src, const unsigned& handle) -> bool {
        auto sources = src;
        std::vector<char*> shader_sources_cstr;
        // cleanup sources by removing the version string starting from the second shader source
        for (auto& shader_source : sources) {
            if (!shader_sources_cstr.empty()) {
                auto shader_source_lines = split(shader_source, '\n');

                auto is_version_line = [](const std::string& s) { return s.find("#version") != std::string::npos; };
                std::erase_if(shader_source_lines, is_version_line);

                shader_source.clear();
                for (auto& line : shader_source_lines) {
                    shader_source += line;
                }
            }
            shader_sources_cstr.push_back(shader_source.data());
        }
        glShaderSource(handle, static_cast<int>(shader_sources_cstr.size()), shader_sources_cstr.data(), nullptr);
        glCompileShader(handle);

        int status;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            std::cerr << "Failed to compile " << to_string(type) << " shader :\n"
                      << get_shader_info_log(handle) << std::endl;
            return false;
        }

        return true;
    }
}

template <>
void Shader::CompiledShaderObject<Shader::Type::Vertex>::destroy() {
    if (handle) glDeleteShader(handle);
}
template <>
Shader::CompiledShaderObject<Shader::Type::Vertex>::CompiledShaderObject(const std::string& src) {
    handle = glCreateShader(GL_VERTEX_SHADER);
    if (!compile(Shader::Type::Vertex, src, handle)) {
        destroy();
    }
}

template <>
void Shader::CompiledShaderObject<Shader::Type::Geometry>::destroy() {
    if (handle) glDeleteShader(handle);
}
template <>
Shader::CompiledShaderObject<Shader::Type::Geometry>::CompiledShaderObject(const std::string& src) {
    handle = glCreateShader(GL_GEOMETRY_SHADER);
    if (!compile(Shader::Type::Geometry, src, handle)) {
        destroy();
    }
}

template <>
void Shader::CompiledShaderObject<Shader::Type::Fragment>::destroy() {
    if (handle) glDeleteShader(handle);
}
template <>
Shader::CompiledShaderObject<Shader::Type::Fragment>::CompiledShaderObject(const std::string& src) {
    handle = glCreateShader(GL_FRAGMENT_SHADER);
    if (!compile(Shader::Type::Fragment, src, handle)) {
        destroy();
    }
}

template <>
Shader::CompiledShaderObject<Shader::Type::Fragment>::CompiledShaderObject(const std::vector<std::string>& src) {
    handle = glCreateShader(GL_FRAGMENT_SHADER);
    if (!compile(Shader::Type::Fragment, src, handle)) {
        destroy();
    }
}

void Shader::Program::create() {
    if (handle != 0) {
        destroy();
    }
    handle = glCreateProgram();
}

void Shader::Program::destroy() {
    if (handle != 0) {
        glDeleteProgram(handle);
    }
}

void Shader::Program::attachShader(const ShaderObject& shader) {
    glAttachShader(handle, getHandle(shader));
}

void Shader::Program::detachShader(const ShaderObject& shader) {
    glDetachShader(handle, getHandle(shader));
}

void Shader::Program::link() const {
    glLinkProgram(handle);
}

void Shader::Program::validate() const {
    glValidateProgram(handle);
}

auto Shader::Program::getInfoLog() const -> std::string {
    int length;
    std::string message;
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
    message.resize(length);
    glGetProgramInfoLog(handle, length, nullptr, message.data());
    return message;
}

void Shader::Program::use() const {
    glUseProgram(handle);
}

auto Shader::Program::getUniformLocation(const std::string& uniform) const -> int {
    return glGetUniformLocation(handle, uniform.c_str());
}

auto Shader::Program::getStatus(unsigned status_type) const -> int {
    int status = 0;
    glGetProgramiv(handle, status_type, &status);
    return status;
}

Shader::Program::operator bool() {
    return handle != 0;
}

Shader::~Shader() {
    if (m_program) m_program.destroy();
}

void Shader::bind() const {
    m_program.use();
}

auto Shader::getUniformLocation(const std::string& uniform) const -> int {
    if (m_uniforms_location.count(uniform) == 0)
        m_uniforms_location[uniform] = m_program.getUniformLocation(uniform);
    return m_uniforms_location.at(uniform);
}

auto Shader::compileVertex(const std::string& src) -> CompiledShaderObject<Type::Vertex> {
    return CompiledShaderObject<Type::Vertex>(src);
}

auto Shader::compileGeometry(const std::string& src) -> CompiledShaderObject<Type::Geometry> {
    return CompiledShaderObject<Type::Geometry>(src);
}

auto Shader::compileFragment(const std::string& src) -> CompiledShaderObject<Type::Fragment> {
    return CompiledShaderObject<Type::Fragment>(src);
}

auto Shader::compileFragment(const std::vector<std::string>& src) -> CompiledShaderObject<Type::Fragment> {
    return CompiledShaderObject<Type::Fragment>(src);
}

void Shader::attach(const ShaderObject& handle) {
    m_program.attachShader(handle);
}

void Shader::detach(const ShaderObject& handle) {
    m_program.detachShader(handle);
}

auto Shader::linkAndValidate() -> bool {
    m_program.link();
    m_program.validate();

    if (m_program.getStatus(GL_LINK_STATUS) == GL_FALSE) {
        std::cerr << "Failed to link shader program :\n"
                  << m_program.getInfoLog() << std::endl;
        return false;
    }

    if (m_program.getStatus(GL_VALIDATE_STATUS) == GL_FALSE) {
        std::cerr << "Failed to validate shader program :\n"
                  << m_program.getInfoLog() << std::endl;
        return false;
    }

    return true;
}
