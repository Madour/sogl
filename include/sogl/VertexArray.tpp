// Created by Modar Nasser on 01/01/2023.

#pragma once

namespace sogl {

    template <typename... AttrTypes>
    VertexArray<AttrTypes...>::VertexArray(Primitive primitive_type) : m_primitive_type(primitive_type) {
        create();
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
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

        enableVertexAttribs<0, AttrTypes...>();

        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    }

    template <typename... AttrTypes>
    template <unsigned I, typename AttribT, typename... AttribTs>
    void VertexArray<AttrTypes...>::enableVertexAttribs(size_t offset) {
        const auto components_nb = sizeof(AttribT) / sizeof(float);

        glEnableVertexAttribArray(I);
        glVertexAttribPointer(I, components_nb, GL_FLOAT, GL_FALSE, sizeof(VertexTuple), (void*) offset);

        if constexpr (I < sizeof...(AttrTypes) - 1) {
            enableVertexAttribs<I + 1, AttribTs...>(offset + sizeof(AttribT));
        }
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::pushTriangle(const std::array<VertexTuple, 3>& vertices) {
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
    void VertexArray<AttrTypes...>::pushQuad(const std::array<VertexTuple, 4>& vertices) {
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
    void VertexArray<AttrTypes...>::bind() const {
        if (m_dirty) {

            m_buffer.clear();
            m_buffer.reserve(m_vertices.size() * sizeof(typename decltype(m_vertices)::value_type));
            for (const auto& vert : m_vertices) {
                pushVertexToBuffer(m_buffer, vert);
            }

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof(typename decltype(m_buffer)::value_type),
                         m_buffer.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(typename decltype(m_indices)::value_type),
                         const_cast<unsigned*>(m_indices.data()), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            m_dirty = false;
        }
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    }

    template <typename... AttrTypes>
    template <unsigned I>
    void VertexArray<AttrTypes...>::pushVertexToBuffer(std::vector<float>& vec, const VertexTuple& vert) const {
        auto& data = std::get<I>(vert);
        auto size = sizeof(data) / sizeof(float);
        const auto* raw = reinterpret_cast<const float*>(&data);
        for (size_t i = 0; i < size; ++i)
            vec.push_back(raw[i]);

        if constexpr(I < sizeof...(AttrTypes) - 1) {
            pushVertexToBuffer<I+1>(vec, vert);
        }
    }

    template <typename... AttrTypes>
    void VertexArray<AttrTypes...>::render() const {
        glDrawElements(static_cast<std::underlying_type_t<Primitive>>(m_primitive_type),
                       m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

}