// Created by Modar Nasser on 01/01/2023.

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <array>

namespace sogl {
    namespace detail {
        struct any_type {
            template <class T>
            constexpr operator T(); // non explicit
        };

        template<class T, typename... Args >
        inline constexpr bool is_brace_constructible_v = requires { T{std::declval<Args>()...}; };
    }

    template<class T>
    auto vertex(T&& object) noexcept {
        using type = std::decay_t<T>;
        using any = detail::any_type;
        if constexpr(detail::is_brace_constructible_v<type, any, any, any, any, any, any>) {
            auto&& [p1, p2, p3, p4, p5, p6] = object;
            return std::make_tuple(p1, p2, p3, p4, p5, p6);
        } else if constexpr(detail::is_brace_constructible_v<type, any, any, any, any, any>) {
            auto&& [p1, p2, p3, p4, p5] = object;
            return std::make_tuple(p1, p2, p3, p4, p5);
        } else if constexpr(detail::is_brace_constructible_v<type, any, any, any, any>) {
            auto&& [p1, p2, p3, p4] = object;
            return std::make_tuple(p1, p2, p3, p4);
        } else if constexpr(detail::is_brace_constructible_v<type, any, any, any>) {
            auto&& [p1, p2, p3] = object;
            return std::make_tuple(p1, p2, p3);
        } else if constexpr(detail::is_brace_constructible_v<type, any, any>) {
            auto&& [p1, p2] = object;
            return std::make_tuple(p1, p2);
        } else if constexpr(detail::is_brace_constructible_v<type, any>) {
            auto&& [p1] = object;
            return std::make_tuple(p1);
        } else {
            return std::make_tuple();
        }
    }
}