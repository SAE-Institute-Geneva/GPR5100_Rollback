#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

namespace core
{
class SystemInterface;

class Engine
{
public:
    void Run();
private:
    void Init();
    void Update(sf::Time time);
    void Destroy();

    std::vector<SystemInterface*> systems_;
    std::unique_ptr<sf::RenderWindow> window_;
};

} // namespace core
