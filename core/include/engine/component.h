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

template<typename T, Component C>
class ComponentManager
{
public:
    ComponentManager(EntityManager& entityManager) : entityManager_(entityManager)
    {
        
    }
    virtual ~ComponentManager() = default;

    void AddComponent(Entity entity);
    void RemoveComponent(Entity entity);

    const T& GetComponent(Entity entity) const;
    T& GetComponent(Entity entity);

protected:
    EntityManager& entityManager_;
    std::vector<T> components_;
};

template <typename T, Component C>
void ComponentManager<T, C>::AddComponent(Entity entity)
{
    const auto currentSize = components_.size();
    if (entity >= currentSize)
    {
        components_.resize(currentSize + currentSize / 2);
    }
    entityManager_.AddComponent(entity, C);
}

template <typename T, Component C>
void ComponentManager<T, C>::RemoveComponent(Entity entity)
{
    entityManager_.RemoveComponent(entity, C);
}

template <typename T, Component C>
const T& ComponentManager<T, C>::GetComponent(Entity entity) const
{
    return components_[entity];
}

template <typename T, Component C>
T& ComponentManager<T, C>::GetComponent(Entity entity)
{
    return components_[entity];
}
} // namespace core
