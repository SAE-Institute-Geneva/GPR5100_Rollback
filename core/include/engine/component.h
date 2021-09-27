#pragma once

#include <cstdint>

namespace core
{
    using Component = std::uint32_t;
    enum class ComponentType : Component
    {
        EMPTY = 1u
    };
} // namespace core
