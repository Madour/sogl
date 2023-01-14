// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <sogl/Vertex.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <array>
#include <tuple>
#include <vector>

namespace sogl {
    namespace detail {
        template <typename T, typename TupleT>
        concept convertible_to_tuple = std::is_aggregate_v<T> && (sizeof(T) == sizeof(TupleT));
    }

    template <typename... AttrTypes>
    class VertexArray {
        using VertexTuple = std::tuple<AttrTypes...>;

    public:
        VertexArray();

        void pushTriangle(const std::array<VertexTuple, 3>& vertices);
        void pushQuad(const std::array<VertexTuple, 4>& vertices);

        template <detail::convertible_to_tuple<VertexTuple> VertexStruct>
        void pushTriangle(const std::array<VertexStruct, 3>& vertices);

        template <detail::convertible_to_tuple<VertexTuple> VertexStruct>
        void pushQuad(const std::array<VertexStruct, 4>& vertices);

        void bind() const;
        void render() const;

    private:
        void create();

        template <unsigned I, typename AttribT, typename... AttribTs>
        void enableVertexAttribs(size_t offset = 0);

        template <unsigned I = 0>
        void pushVertexToBuffer(std::vector<float>& vec, const VertexTuple& vert) const;

        unsigned m_vao = 0;
        unsigned m_vbo = 0;
        unsigned m_ibo = 0;

        std::vector<VertexTuple> m_vertices;
        std::vector<unsigned> m_indices;

        mutable std::vector<float> m_buffer;
        mutable bool m_dirty = false;

    };
}

#include "VertexArray.tpp"
