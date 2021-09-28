#pragma once

namespace sf
{
class RenderWindow;
}

namespace core
{
class DrawInterface
{
public:
    virtual void Draw(sf::RenderTarget& window) = 0;
};

class DrawImGuiInterface
{
public:
    virtual void DrawImGui() = 0;
};
}
