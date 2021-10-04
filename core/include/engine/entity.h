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
    EntityManager();
    EntityManager(std::size_t reservedSize);

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void AddComponent(Entity entity, EntityMask mask);
    void RemoveComponent(Entity entity, EntityMask mask);
    bool HasComponent(Entity entity, EntityMask mask) const;
    bool EntityExists(Entity entity) const;

    [[nodiscard]] std::size_t GetEntitiesSize() const;

    static constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();
    static constexpr EntityMask INVALID_ENTITY_MASK = 0u;
private:
    std::vector<EntityMask> entityMasks_;
};

} // namespace core
