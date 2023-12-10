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
            Vertex,
            Geometry,
            Fragment,
        };

    private:
        struct ShaderHandle {
            unsigned shader = 0;
            operator bool() const { return shader != 0; }
        };

        template <Type T>
        class ShaderObject {
            friend Shader;
        public:
            void destroy();

            operator bool() const { return handle; }

        private:
            ShaderObject();
            ShaderObject(void*) {}; // dummy ctor for creating an empty ShaderObject
            ShaderObject(const ShaderObject&) = delete;
            ShaderObject(ShaderObject&&) = default;
            ShaderObject& operator=(const ShaderObject&) = delete;
            ShaderObject& operator=(ShaderObject&&) = default;

            ShaderHandle handle;
        };

    public:
        Shader();
        ~Shader();
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        static auto compileVertex(const std::string& src) -> ShaderObject<Type::Vertex>;
        static auto compileGeometry(const std::string& src) -> ShaderObject<Type::Geometry>;
        static auto compileFragment(const std::string& src) -> ShaderObject<Type::Fragment>;

        template <typename... TShaderObjects>
        auto load(TShaderObjects&... shader_objects) -> bool {
            attach_r(shader_objects...);
            auto ret = linkAndValidate();
            detach_r(shader_objects...);
            return ret;
        }

        void bind() const;

        [[nodiscard]]
        auto getUniformLocation(const std::string& uniform) const -> int;

        template <typename T>
        void setUniform(const std::string& uniform, const T& val);

        template <typename T>
        void setUniform(const std::string& uniform, const std::initializer_list<T>& arr);

    private:
        static auto compile(Type type, const std::string& src, ShaderHandle& handle) -> bool;

        template <typename TShaderObject, typename... TShaderObjects>
        void attach_r(TShaderObject& shader_object, TShaderObjects&... shader_objects) {
            attach(shader_object.handle);

            if constexpr(sizeof...(shader_objects) > 0) {
                attach_r(shader_objects...);
            }
        }

        template <typename TShaderObject, typename... TShaderObjects>
        void detach_r(TShaderObject& shader_object, TShaderObjects&... shader_objects) {
            detach(shader_object.handle);

            if constexpr(sizeof...(shader_objects) > 0) {
                detach_r(shader_objects...);
            }
        }

        void attach(ShaderHandle& handle);
        void detach(ShaderHandle& handle);
        auto linkAndValidate() -> bool;

        unsigned m_program =  0;
        mutable std::unordered_map<std::string, int> m_uniforms_location;
    };

#include "Shader.tpp"

}
