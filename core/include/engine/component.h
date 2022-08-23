#pragma once

#include <cstdint>
#include <engine/globals.h>
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

    /**
     * \brief ComponentManager is a class that owns Component in a contiguous array. Component indexing is done with an Entity.
     * \tparam T type of the component
     * \tparam C unique binary flag of the component. This will be set in the EntityMask of the EntityManager when added.
     */
    template<typename T, Component C>
    class ComponentManager
    {
    public:
        ComponentManager(EntityManager& entityManager) : entityManager_(entityManager)
        {
            components_.resize(entityInitNmb);
        }
        virtual ~ComponentManager() = default;

        ComponentManager(const ComponentManager&) = delete;
        ComponentManager& operator=(ComponentManager&) = delete;
        ComponentManager(ComponentManager&&) = delete;
        ComponentManager& operator=(ComponentManager&&) = delete;

        /**
         * \brief AddComponent is a method that sets the flag C in the EntityManager and resize if need the components_ array.
         * \param entity will have its flag C added in EntityManager
         */
        virtual void AddComponent(Entity entity);
        /**
         * \brief RemoveComponent is a method that unsets the flag C in the EntityManager
         * \param entity will have its flag C removed
         */
        virtual void RemoveComponent(Entity entity);

        [[nodiscard]] const T& GetComponent(Entity entity) const;
        [[nodiscard]] T& GetComponent(Entity entity);

        void SetComponent(Entity entity, const T& value);

        [[nodiscard]] const std::vector<T>& GetAllComponents() const;
        void CopyAllComponents(const std::vector<T>& components);
    protected:
        EntityManager& entityManager_;
        std::vector<T> components_;
    };

    template <typename T, Component C>
    void ComponentManager<T, C>::AddComponent(Entity entity)
    {
        // Resize components array if too small
        auto currentSize = components_.size();
        while (entity >= currentSize)
        {
            auto newSize = currentSize + currentSize / 2;
            components_.resize(newSize);
            currentSize = newSize;
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
    const std::vector<T>& ComponentManager<T, C>::GetAllComponents() const
    {
        return components_;
    }

    template <typename T, Component C>
    void ComponentManager<T, C>::CopyAllComponents(const std::vector<T>& components)
    {
        components_ = components;
    }
} // namespace core
