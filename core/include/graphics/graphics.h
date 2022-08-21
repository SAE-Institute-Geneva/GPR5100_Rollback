#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

namespace core
{
class DrawInterface
{
public:
    virtual void Draw(sf::RenderTarget& renderTarget) = 0;
};

class DrawImGuiInterface
{
public:
    virtual void DrawImGui() = 0;
};
}
