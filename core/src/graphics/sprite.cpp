#include <graphics/sprite.h>

namespace core
{
void SpriteManager::SetTexture(Entity entity, const sf::Texture& texture)
{
    components_[entity].setTexture(texture);
}

void SpriteManager::Draw(sf::RenderTarget& window)
{
    for(Entity entity = 0; entity < components_.size(); entity++)
    {
        if(entityManager_.HasComponent(entity, static_cast<Component>(ComponentType::SPRITE)))
        {
            window.draw(components_[entity]);
        }
    }
}
} // namespace core
