#pragma once
#include "component.h"
#include "maths/angle.h"
#include "maths/vec2.h"

#include <engine/entity.h>

namespace core
{

class PositionManager : public ComponentManager<Vec2f, static_cast<std::uint32_t>(ComponentType::POSITION)>
{
    using ComponentManager::ComponentManager;
};

class ScaleManager : public ComponentManager<Vec2f, static_cast<std::uint32_t>(ComponentType::SCALE)>
{
    using ComponentManager::ComponentManager;
};

class RotationManager : public ComponentManager<degree_t, static_cast<std::uint32_t>(ComponentType::ROTATION)>
{
    using ComponentManager::ComponentManager;
};

class TransformManager
{
public:
    TransformManager(EntityManager& entityManager);

    Vec2f GetPosition(Entity entity) const;
    const std::vector<Vec2f>& GetPositions() const;
    void SetPosition(Entity entity, Vec2f position);

    Vec2f GetScale(Entity entity) const;
    const std::vector<Vec2f>& GetScales() const;
    void SetScale(Entity entity, Vec2f scale);

    degree_t GetRotation(Entity entity) const;
    const std::vector<degree_t>& GetRotations() const;
    void SetRotation(Entity entity, degree_t rotation);

    void AddComponent(Entity entity);
    void RemoveComponent(Entity entity);
    
private:
    PositionManager positionManager_;
    ScaleManager scaleManager_;
    RotationManager rotationManager_;
};

}
