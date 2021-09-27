#pragma once

#include <cstdint>

namespace core
{
    using Component = std::uint32_t;
    enum class ComponentType : Component
    {
        EMPTY = 1u,
        POSITION = 1u << 1u,
        SCALE = 1u << 2u,
        ROTATION = 1u << 3u,
        TRANSFORM = POSITION | SCALE | ROTATION,
        OTHER_TYPE = 1u << 4u
    };
} // namespace core
