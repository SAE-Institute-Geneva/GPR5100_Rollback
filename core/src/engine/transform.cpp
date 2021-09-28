#pragma once

#include <engine/transform.h>

namespace core
{
TransformManager::TransformManager(EntityManager& entityManager) :
    ComponentManager(entityManager),
    positionManager_(entityManager),
    scaleManager_(entityManager),
    rotationManager_(entityManager)
{

}
}
