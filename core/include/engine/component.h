#pragma once

#include <cstdint>

#include <engine/entity.h>

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
        SPRITE = 1u << 4u,
        BODY2D = 1u << 5u,
        BOX_COLLIDER2D = 1u << 6u,
        OTHER_TYPE = 1u << 7u
    };

template<typename T, Component C>
class ComponentManager
{
public:
    ComponentManager(EntityManager& entityManager) : entityManager_(entityManager)
    {
        
    }
    virtual ~ComponentManager() = default;

    ComponentManager(const ComponentManager& a) = delete;
    ComponentManager& operator=(ComponentManager&) = delete;
    ComponentManager(ComponentManager&&) = delete;
    ComponentManager& operator=(ComponentManager&&) = delete;

    void AddComponent(Entity entity);
    void RemoveComponent(Entity entity);

    [[nodiscard]] const T& GetComponent(Entity entity) const;
    [[nodiscard]] T& GetComponent(Entity entity);

    void SetComponent(Entity entity, const T& value);

    [[nodiscard]] const std::vector<T>& GetComponents() const;
    void SetComponents(const std::vector<T>& components);
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

template <typename T, Component C>
void ComponentManager<T, C>::SetComponent(Entity entity, const T& value)
{
    components_[entity] = value;
}

template <typename T, Component C>
const std::vector<T>& ComponentManager<T, C>::GetComponents() const
{
    return components_;
}

template <typename T, Component C>
void ComponentManager<T, C>::SetComponents(const std::vector<T>& components)
{
    components_ = components;
}
} // namespace core
