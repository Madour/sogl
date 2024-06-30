// Created by Modar Nasser on 05/03/2022.

#pragma once

#include <sogl/Vertex.hpp>

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace sogl {
    namespace detail {
        template <typename T, typename TupleT>
        concept convertible_to_tuple = std::is_aggregate_v<T> && (sizeof(T) == sizeof(TupleT));
    }

    template <typename... AttrTypes>
    class VertexArray {
        using AttrTuple = std::tuple<AttrTypes...>;

        template <size_t I>
        using AttrTupleElement = std::tuple_element_t<I, AttrTuple>;

        static constexpr auto AttrTupleSize = std::tuple_size_v<AttrTuple>;

        template<typename T, typename... Ts>
        struct size_of
        {
            static constexpr size_t value = size_of<T>::value + size_of<Ts...>::value;
        };

        template <typename T>
        struct size_of<T>
        {
            static constexpr size_t value = sizeof(T);
        };

        static constexpr auto SizeOfAttrTuple = size_of<AttrTypes...>::value;

        class Vertex {
        public:
            Vertex(const AttrTuple& tuple);

            template <size_t I>
            auto get() -> AttrTupleElement<I>&;

            template <size_t I>
            auto get() const -> const AttrTupleElement<I>&;

            template <typename VertexClass>
            auto as() -> VertexClass&;

        private:
            template <size_t I = 0>
            void initFrom(const AttrTuple& tuple);

            template <size_t I>
            constexpr auto offset() const -> size_t;

            template <size_t I>
            constexpr auto size() const -> size_t;

            std::array<std::uint8_t, SizeOfAttrTuple> m_bytes;
        };

    public:
        explicit VertexArray(Primitive primitive_type = Primitive::Triangles);
        ~VertexArray();

        void setPrimitiveType(sogl::Primitive primitive_type);

        void clear();

        void push(const AttrTuple& v);
        void pushTriangle(const std::array<AttrTuple, 3>& vertices);
        void pushQuad(const std::array<AttrTuple, 4>& vertices);

        template <detail::convertible_to_tuple<AttrTuple> VertexStruct>
        void push(const VertexStruct& v) {
            push(vertex(v));
        }

        template <detail::convertible_to_tuple<AttrTuple> VertexStruct>
        void pushTriangle(const std::array<VertexStruct, 3>& vertices) {
            pushTriangle({{
                vertex(vertices[0]),
                vertex(vertices[1]),
                vertex(vertices[2])
            }});
        }

        template <detail::convertible_to_tuple<AttrTuple> VertexStruct>
        void pushQuad(const std::array<VertexStruct, 4>& vertices) {
            pushQuad({{
                vertex(vertices[0]),
                vertex(vertices[1]),
                vertex(vertices[2]),
                vertex(vertices[3])
            }});
        }

        template <typename VertexT = Vertex>
        auto get(size_t i) -> VertexT&;

        template <typename VertexT = Vertex>
        auto get(size_t i) const -> const VertexT&;

        void bind() const;
        void render() const;

    private:
        void create();
        void destroy();

        template <unsigned I, typename AttribT, typename... AttribTs>
        void enableVertexAttribs(size_t offset = 0);

        template <unsigned I = 0>
        void pushVertexToBuffer(std::vector<float>& buff, const Vertex& vert) const;

        Primitive m_primitive_type;

        unsigned m_vao = 0;
        unsigned m_vbo = 0;
        unsigned m_ibo = 0;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned> m_indices;

        mutable std::vector<float> m_buffer;
        mutable bool m_dirty = false;
    };
}

#include "VertexArray.tpp"
