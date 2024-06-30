// Created by Modar Nasser on 01/01/2023.

#pragma once

namespace sogl {

    template <typename... AttrTypes>
    VertexArray<AttrTypes...>::Vertex::Vertex(const VertexArray<AttrTypes...>::AttrTuple& tuple) {
        initFrom(tuple);
    }

    template <typename... AttrTypes>
    template <size_t I>
    auto VertexArray<AttrTypes...>::Vertex::get() -> AttrTupleElement<I>& {
        return *reinterpret_cast<AttrTupleElement<I>*>(m_bytes.data() + offset<I>());
    }

    template <typename... AttrTypes>
    template <size_t I>
    auto VertexArray<AttrTypes...>::Vertex::get() const -> const AttrTupleElement<I>& {
        return *reinterpret_cast<const AttrTupleElement<I>*>(m_bytes.data() + offset<I>());
    }

    template <typename... AttrTypes>
    template <typename VertexClass>
    auto VertexArray<AttrTypes...>::Vertex::as() -> VertexClass& {
        static_assert(sizeof(VertexClass) == SizeOfAttrTuple,
                      "Provided type is not equivalent to the Vertex data types");

        return *reinterpret_cast<VertexClass*>(m_bytes.data());
    }

    template <typename... AttrTypes>
    template <size_t I>
    constexpr auto VertexArray<AttrTypes...>::Vertex::offset() const -> size_t {
        if constexpr(I > 0) {
            return size<I - 1>() + offset<I - 1>();
        }
        else {
            return 0;
        }
    }

    template <typename... AttrTypes>
    template <size_t I>
    constexpr auto VertexArray<AttrTypes...>::Vertex::size() const -> size_t {
        return sizeof(AttrTupleElement<I>);
    }

    template <typename... AttrTypes>
    template <size_t I>
    void VertexArray<AttrTypes...>::Vertex::initFrom(const AttrTuple& tuple) {
        get<I>() = std::get<I>(tuple);

        if constexpr(I < AttrTupleSize - 1) {
            initFrom<I + 1>(tuple);
        }
    };

    template <typename... AttrTypes>
    VertexArray<AttrTypes...>::VertexArray(Primitive primitive_type) : m_primitive_type(primitive_type) {
        create();
    }

    template <typename... AttrTypes>
    VertexArray<AttrTypes...>::~VertexArray() {
        if (m_vao) {
            destroy();
        }
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::setPrimitiveType(sogl::Primitive primitive_type) {
        m_primitive_type = primitive_type;
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::create() {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        enableVertexAttribs<0, AttrTypes...>();

        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::destroy() {
        glDeleteBuffers(1, &m_ibo);
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::clear() {
        m_vertices.clear();
        m_indices.clear();
        m_buffer.clear();
        m_dirty = true;
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::push(const AttrTuple& v) {
        m_indices.push_back(m_vertices.size());
        m_vertices.push_back(v);

        m_dirty = true;
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::pushTriangle(const std::array<AttrTuple, 3>& vertices) {
        auto index_offset = m_vertices.size();

        for (const auto& v : vertices) {
            m_vertices.push_back(v);
        }

        m_indices.push_back(index_offset);
        m_indices.push_back(index_offset + 1);
        m_indices.push_back(index_offset + 2);

        m_dirty = true;
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::pushQuad(const std::array<AttrTuple, 4>& vertices) {
        auto index_offset = m_vertices.size();

        for (const auto& v : vertices) {
            m_vertices.push_back(v);
        }

        m_indices.push_back(index_offset);
        m_indices.push_back(index_offset + 1);
        m_indices.push_back(index_offset + 2);
        m_indices.push_back(index_offset);
        m_indices.push_back(index_offset + 2);
        m_indices.push_back(index_offset + 3);

        m_dirty = true;
    }

    template <typename... AttrTypes>
    template <typename VertexT>
    auto VertexArray<AttrTypes...>::get(size_t i) -> VertexT& {
        if constexpr(std::is_same_v<VertexT, Vertex>) {
            return m_vertices.at(i);
        }
        else {
            return m_vertices.at(i).as<VertexT>();
        }
    }

    template <typename... AttrTypes>
    template <typename VertexT>
    auto VertexArray<AttrTypes...>::get(size_t i) const -> const VertexT& {
        if constexpr(std::is_same_v<VertexT, Vertex>) {
            return m_vertices.at(i);
        }
        else {
            return m_vertices.at(i).as<VertexT>();
        }
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::bind() const {
        if (m_dirty) {

            m_buffer.clear();
            m_buffer.reserve(m_vertices.size() * sizeof(typename decltype(m_vertices)::value_type));
            for (const auto& vert : m_vertices) {
                pushVertexToBuffer(m_buffer, vert);
            }

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof(typename decltype(m_buffer)::value_type),
                         m_buffer.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(typename decltype(m_indices)::value_type),
                         const_cast<unsigned*>(m_indices.data()), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            m_dirty = false;
        }
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    }

    template <typename... AttrTypes>
    template <unsigned I, typename AttribT, typename... AttribTs>
    void VertexArray<AttrTypes...>::enableVertexAttribs(size_t offset) {
        static_assert(sizeof(AttribT) % sizeof(float) == 0, "Vertex attributes must be float values");

        const auto components_nb = sizeof(AttribT) / sizeof(float);

        glEnableVertexAttribArray(I);
        glVertexAttribPointer(I, components_nb, GL_FLOAT, GL_FALSE, sizeof(AttrTuple), (void*) offset);

        if constexpr (I < AttrTupleSize - 1) {
            enableVertexAttribs<I + 1, AttribTs...>(offset + sizeof(AttribT));
        }
    }

    template <typename... AttrTypes>
    template <unsigned I>
    void VertexArray<AttrTypes...>::pushVertexToBuffer(std::vector<float>& buff, const Vertex& vert) const {
        const auto& data = vert.get<I>();
        const auto size = sizeof(data) / sizeof(float);
        const auto* raw = reinterpret_cast<const float*>(&data);

        for (size_t i = 0; i < size; ++i)
            buff.push_back(raw[i]);

        if constexpr(I < AttrTupleSize - 1) {
            pushVertexToBuffer<I + 1>(buff, vert);
        }
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::render() const {
        glDrawElements(static_cast<std::underlying_type_t<Primitive>>(m_primitive_type),
                       m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

}
