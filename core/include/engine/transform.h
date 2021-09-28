#pragma once
#include "component.h"
#include "maths/angle.h"
#include "maths/vec2.h"

namespace core
{
class EntityManager;

class PositionManager : public ComponentManager<Vec2f, ComponentType::POSITION>
{
    using ComponentManager::ComponentManager;
};

class ScaleManager : public ComponentManager<Vec2f, ComponentType::SCALE>
{
    using ComponentManager::ComponentManager;
};

class RotationManager : public ComponentManager<degree_t, ComponentType::ROTATION>
{
    using ComponentManager::ComponentManager;
};

class TransformManager
{
public:
    TransformManager(EntityManager& entityManager);
private:
    PositionManager positionManager_;
    ScaleManager scaleManager_;
    RotationManager rotationManager_;
};

}
