// Created by Modar Nasser on 28/02/2022.

#include <sogl/Shader.hpp>

#include <GL/glew.h>

#include <iostream>
#include <string>

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

    auto get_program_info_log(unsigned program) -> std::string{
        int length;
        std::string message;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        message.resize(length);
        glGetProgramInfoLog(program, length, nullptr, message.data());
        return message;
    }
}

template <>
Shader::ShaderObject<Shader::Type::Vertex>::ShaderObject() {
    handle.shader = glCreateShader(GL_VERTEX_SHADER);
}
template <>
void Shader::ShaderObject<Shader::Type::Vertex>::destroy() {
    if (handle) glDeleteShader(handle.shader);
}

template <>
Shader::ShaderObject<Shader::Type::Geometry>::ShaderObject() {
    handle.shader = glCreateShader(GL_GEOMETRY_SHADER);
}
template <>
void Shader::ShaderObject<Shader::Type::Geometry>::destroy() {
    if (handle) glDeleteShader(handle.shader);
}

template <>
Shader::ShaderObject<Shader::Type::Fragment>::ShaderObject() {
    handle.shader = glCreateShader(GL_FRAGMENT_SHADER);
}
template <>
void Shader::ShaderObject<Shader::Type::Fragment>::destroy() {
    if (handle) glDeleteShader(handle.shader);
}

Shader::Shader() {
    m_program = glCreateProgram();
}

Shader::~Shader() {
    if (m_program) glDeleteProgram(m_program);
}

void Shader::bind() const {
    glUseProgram(m_program);
}

auto Shader::getUniformLocation(const std::string& uniform) const -> int {
    if (m_uniforms_location.count(uniform) == 0)
        m_uniforms_location[uniform] = glGetUniformLocation(m_program, uniform.c_str());
    return m_uniforms_location.at(uniform);
}

auto Shader::compileVertex(const std::string& src) -> ShaderObject<Type::Vertex> {
    ShaderObject<Type::Vertex> shader_obj;
    if (compile(Type::Vertex, src, shader_obj.handle))
        return shader_obj;
    else
        return ShaderObject<Type::Vertex>(nullptr);
}

auto Shader::compileGeometry(const std::string& src) -> ShaderObject<Type::Geometry> {
    ShaderObject<Type::Geometry> shader_obj;
    if (compile(Type::Geometry, src, shader_obj.handle))
        return shader_obj;
    else
        return ShaderObject<Type::Geometry>(nullptr);
}

auto Shader::compileFragment(const std::string& src) -> ShaderObject<Type::Fragment> {
    ShaderObject<Type::Fragment> shader_obj;
    if (compile(Type::Fragment, src, shader_obj.handle))
        return shader_obj;
    else
        return ShaderObject<Type::Fragment>(nullptr);
}

auto Shader::compile(Type type, const std::string& src, ShaderHandle& handle) -> bool {
    auto& shader = handle.shader;
    auto* src_c = src.c_str();
    glShaderSource(shader, 1, &src_c, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    auto to_string = [] (const Shader::Type& t) -> std::string {
        switch (t) {
            case Shader::Type::Vertex: return "Vertex";
            case Shader::Type::Fragment: return "Fragment";
            case Shader::Type::Geometry: return "Geometry";
            default: return "Unknown";
        }
    };

    if (status == GL_FALSE) {
        std::cerr << "Failed to compile " << to_string(type) << " shader :\n"
                  << get_shader_info_log(shader) << std::endl;
        return false;
    }

    return true;
}

void Shader::attach(ShaderHandle& handle) {
    glAttachShader(m_program, handle.shader);
}

void Shader::detach(ShaderHandle& handle) {
    glDetachShader(m_program, handle.shader);
}

auto Shader::linkAndValidate() -> bool {
    glLinkProgram(m_program);
    glValidateProgram(m_program);

    int link_status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        std::cerr << "Failed to link shader program :\n"
                  << get_program_info_log(m_program) << std::endl;
        return false;
    }

    int validate_status;
    glGetProgramiv(m_program, GL_VALIDATE_STATUS, &validate_status);
    if (validate_status == GL_FALSE) {
        std::cerr << "Failed to validate shader program :\n"
                  << get_program_info_log(m_program) << std::endl;
        return false;
    }

    return true;
}
