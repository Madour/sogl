// Created by Modar Nasser on 28/02/2022.

#pragma once

#include <sogl/GlObject.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <concepts>
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
            Vertex,
            Geometry,
            Fragment,
        };

    private:
        class ShaderObject : public GlObject {
        public:
            operator bool() const { return handle != 0; }
        };

        template <Type T>
        class CompiledShaderObject : public ShaderObject {
        public:
            CompiledShaderObject(const std::string& src);
            void destroy();
        };

        class Program : public GlObject {
        public:
            void create();
            void destroy();
            void attachShader(const ShaderObject& shader_handle);
            void detachShader(const ShaderObject& shader_handle);
            void link() const;
            void validate() const;
            auto getInfoLog() const -> std::string;
            void use() const;
            auto getUniformLocation(const std::string& uniform) const -> int;
            auto getStatus(unsigned status_type) const -> int;
            operator bool();
        };

    public:
        Shader() = default;
        ~Shader();
        Shader(const Shader&) = delete;
        Shader(Shader&&) = default;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = default;

        static auto compileVertex(const std::string& src) -> CompiledShaderObject<Type::Vertex>;
        static auto compileGeometry(const std::string& src) -> CompiledShaderObject<Type::Geometry>;
        static auto compileFragment(const std::string& src) -> CompiledShaderObject<Type::Fragment>;

        template <typename... TShaderObjects>
        auto load(TShaderObjects&... shader_objects) -> bool {
            m_program.create();
            attach_r(shader_objects...);
            auto ret = linkAndValidate();
            detach_r(shader_objects...);
            return ret;
        }

        void bind() const;

        [[nodiscard]]
        auto getUniformLocation(const std::string& uniform) const -> int;

        template <typename T>
        void setUniform(const std::string& uniform, const T& val) const;

        template <typename T>
        void setUniform(const std::string& uniform, const std::initializer_list<T>& arr) const;

    private:
        template <typename TShaderObject, typename... TShaderObjects> requires std::derived_from<TShaderObject, ShaderObject>
        void attach_r(TShaderObject& shader_object, TShaderObjects&... shader_objects) {
            attach(shader_object);

            if constexpr(sizeof...(shader_objects) > 0) {
                attach_r(shader_objects...);
            }
        }

        template <typename TShaderObject, typename... TShaderObjects>requires std::derived_from<TShaderObject, ShaderObject>
        void detach_r(TShaderObject& shader_object, TShaderObjects&... shader_objects) {
            detach(shader_object);

            if constexpr(sizeof...(shader_objects) > 0) {
                detach_r(shader_objects...);
            }
        }

        void attach(const ShaderObject& handle);
        void detach(const ShaderObject& handle);
        auto linkAndValidate() -> bool;

        Program m_program;
        mutable std::unordered_map<std::string, int> m_uniforms_location;
    };

#include "Shader.tpp"

}
