// Created by Modar Nasser on 28/02/2022.

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

#if defined(EMSCRIPTEN)
#define GLSL(version, src) "#version 300 es\n" #src
#else
#define GLSL(version, src) "#version "#version"\n" #src
#endif

namespace sogl {

    class Shader {
    public:
        enum class Type : int {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Count
        };

        Shader() = default;
        ~Shader();
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(const Shader&&) = delete;

        auto load(const char* vert_src, const char* frag_src) -> bool;
        void bind() const;

        [[nodiscard]] auto getUniformLocation(const std::string& uniform) const -> int;

        template <typename T>
        void setUniform(const std::string& uniform, const T& val);

        template <typename T, glm::length_t S>
        void setUniform(const std::string& uniform, const glm::vec<S, T, glm::defaultp>& vec);

        template <typename T>
        void setUniform(const std::string& uniform, const std::initializer_list<T>& arr);

        template <typename T, glm::length_t S>
        void setUniform(const std::string& uniform, const std::initializer_list<glm::vec<S, T, glm::defaultp>>& arr);

    private:
        auto compile(Type type, const char* src) -> bool;
        void unload();

        unsigned m_program =  0;
        std::unordered_map<Type, unsigned> m_shaders;
        mutable std::unordered_map<std::string, int> m_uniforms_location;
    };

#include "Shader.tpp"

}
