// Created by Modar Nasser on 20/01/2024.

#pragma once

struct GlObject {
    GlObject() = default;
    GlObject(const GlObject&) = delete;
    GlObject(GlObject&& other) { handle = other.handle; other.handle = 0; }
    GlObject& operator=(const GlObject&) = delete;
    GlObject& operator=(GlObject&& other) { handle = other.handle; other.handle = 0; }

protected:
    auto getHandle(const GlObject& other) const -> unsigned { return other.handle; }

    unsigned handle = 0;
};
