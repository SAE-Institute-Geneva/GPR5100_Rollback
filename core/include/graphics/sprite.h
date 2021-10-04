#pragma once

#include "engine/component.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "graphics.h"

namespace core
{
    class TransformManager;
class SpriteManager : public ComponentManager<sf::Sprite, static_cast<std::uint32_t>(ComponentType::SPRITE)>, public DrawInterface
{
public:
    SpriteManager(EntityManager& entityManager, TransformManager& transformManager) :
        ComponentManager(entityManager),
        transformManager_(transformManager)
    {
        
    }
    void SetOrigin(Entity entity, sf::Vector2f origin);
    void SetTexture(Entity entity, const sf::Texture& texture);
    void SetCenter(sf::Vector2f center) { center_ = center; }
    void Draw(sf::RenderTarget& window) override;

protected:
    TransformManager& transformManager_;
    sf::Vector2f center_{};
    
};

}
