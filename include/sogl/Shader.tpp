// Created by Modar Nasser on 28/02/2022.

template <typename T>
void Shader::setUniform(const std::string& uniform, const T& val) const {
    auto location = getUniformLocation(uniform);
    if constexpr(std::is_same_v<T, float>) {
        glUniform1f(location, val);
    } else if constexpr(std::is_same_v<T, int>) {
        glUniform1i(location, val);
    } else if constexpr(std::is_same_v<T, unsigned>) {
        glUniform1ui(location, val);
    } else if constexpr(std::is_same_v<T, glm::vec<1, float>>) {
        glUniform1f(location, val.x);
    } else if constexpr(std::is_same_v<T, glm::vec<2, float>>) {
        glUniform2f(location, val.x, val.y);
    } else if constexpr(std::is_same_v<T, glm::vec<3, float>>) {
        glUniform3f(location, val.x, val.y, val.z);
    } else if constexpr(std::is_same_v<T, glm::vec<4, float>>) {
        glUniform4f(location, val.x, val.y, val.z, val.w);
    } else if constexpr(std::is_same_v<T, glm::vec<1, int>>) {
        glUniform1i(location, val.x);
    } else if constexpr(std::is_same_v<T, glm::vec<2, int>>) {
        glUniform2i(location, val.x, val.y);
    } else if constexpr(std::is_same_v<T, glm::vec<3, int>>) {
        glUniform3i(location, val.x, val.y, val.z);
    } else if constexpr(std::is_same_v<T, glm::vec<4, int>>) {
        glUniform4i(location, val.x, val.y, val.z, val.w);
    } else if constexpr(std::is_same_v<T, glm::vec<1, unsigned>>) {
        glUniform1ui(location, val.x);
    } else if constexpr(std::is_same_v<T, glm::vec<2, unsigned>>) {
        glUniform2ui(location, val.x, val.y);
    } else if constexpr(std::is_same_v<T, glm::vec<3, unsigned>>) {
        glUniform3ui(location, val.x, val.y, val.z);
    } else if constexpr(std::is_same_v<T, glm::vec<4, unsigned>>) {
        glUniform4ui(location, val.x, val.y, val.z, val.w);
    } else if constexpr(std::is_same_v<T, glm::mat2x2>) {
        glUniformMatrix2fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat2x3>) {
        glUniformMatrix2x3fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat2x4>) {
        glUniformMatrix2x4fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat3x2>) {
        glUniformMatrix3x2fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat3x3>) {
        glUniformMatrix3fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat3x4>) {
        glUniformMatrix3x4fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat4x2>) {
        glUniformMatrix4x2fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat4x3>) {
        glUniformMatrix4x3fv(location, 1, false, reinterpret_cast<const float*>(&val));
    } else if constexpr(std::is_same_v<T, glm::mat4x4>) {
        glUniformMatrix4fv(location, 1, false, reinterpret_cast<const float*>(&val));
    }
}

template <typename T>
void Shader::setUniform(const std::string& uniform, const std::initializer_list<T>& arr) const {
    auto location = getUniformLocation(uniform);
    if constexpr(std::is_same_v<T, float>) {
        glUniform1fv(location, std::size(arr), std::data(arr));
    } else if constexpr(std::is_same_v<T, int>) {
        glUniform1iv(location, std::size(arr), std::data(arr));
    } else if constexpr(std::is_same_v<T, unsigned>) {
        glUniform1uiv(location, std::size(arr), std::data(arr));
    } else if constexpr(std::is_same_v<T, glm::vec<1, float>>) {
        glUniform1f(location, std::size(arr), reinterpret_cast<const T*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<2, float>>) {
        glUniform2f(location, std::size(arr), reinterpret_cast<const T*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<3, float>>) {
        glUniform3f(location, std::size(arr), reinterpret_cast<const T*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<4, float>>) {
        glUniform4f(location, std::size(arr), reinterpret_cast<const T*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<1, int>>) {
        glUniform1i(location, std::size(arr), reinterpret_cast<const T*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<2, int>>) {
        glUniform2i(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<3, int>>) {
        glUniform3i(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<4, int>>) {
        glUniform4i(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<1, unsigned>>) {
        glUniform1ui(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<2, unsigned>>) {
        glUniform2ui(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<3, unsigned>>) {
        glUniform3ui(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::vec<4, unsigned>>) {
        glUniform4ui(location, std::size(arr), reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat2x2>) {
        glUniformMatrix2fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat2x3>) {
        glUniformMatrix2x3fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat2x4>) {
        glUniformMatrix2x4fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat3x2>) {
        glUniformMatrix3x2fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat3x3>) {
        glUniformMatrix3fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat3x4>) {
        glUniformMatrix3x4fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat4x2>) {
        glUniformMatrix4x2fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat4x3>) {
        glUniformMatrix4x3fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    } else if constexpr(std::is_same_v<T, glm::mat4x4>) {
        glUniformMatrix4fv(location, std::size(arr), false, reinterpret_cast<const float*>(std::data(arr)));
    }
}
