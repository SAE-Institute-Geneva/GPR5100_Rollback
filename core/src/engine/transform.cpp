#pragma once

#include <engine/transform.h>

namespace core
{
TransformManager::TransformManager(EntityManager& entityManager) :
    positionManager_(entityManager),
    scaleManager_(entityManager),
    rotationManager_(entityManager)
{

}

Vec2f TransformManager::GetPosition(Entity entity) const
{
    return positionManager_.GetComponent(entity);
}

const std::vector<Vec2f>& TransformManager::GetPositions() const
{
    return positionManager_.GetComponents();
}

const std::vector<Vec2f>& TransformManager::GetScales() const
{
    return scaleManager_.GetComponents();
}

const std::vector<degree_t>& TransformManager::GetRotations() const
{
    return rotationManager_.GetComponents();
}

void TransformManager::SetPosition(Entity entity, Vec2f position)
{
    positionManager_.SetComponent(entity, position);
}

Vec2f TransformManager::GetScale(Entity entity) const
{
    return scaleManager_.GetComponent(entity);
}

void TransformManager::SetScale(Entity entity, Vec2f scale)
{
    scaleManager_.SetComponent(entity, scale);
}

degree_t TransformManager::GetRotation(Entity entity) const
{
    return rotationManager_.GetComponent(entity);
}

void TransformManager::SetRotation(Entity entity, degree_t rotation)
{
    rotationManager_.SetComponent(entity, rotation);
}

void TransformManager::AddComponent(Entity entity)
{
    positionManager_.AddComponent(entity);
    scaleManager_.AddComponent(entity);
    rotationManager_.AddComponent(entity);
}

void TransformManager::RemoveComponent(Entity entity)
{
    positionManager_.AddComponent(entity);
    scaleManager_.AddComponent(entity);
    rotationManager_.AddComponent(entity);
}
}
