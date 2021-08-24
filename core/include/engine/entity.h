#pragma once

#include <cstdint>
#include <vector>

namespace core
{
    
using Entity = std::uint32_t;
using EntityMask = std::uint32_t;

class EntityManager
{
public:
    EntityManager() = default;
    EntityManager(std::size_t reservedSize);
private:
    std::vector<EntityMask> entityMasks_;
};

} // namespace core
