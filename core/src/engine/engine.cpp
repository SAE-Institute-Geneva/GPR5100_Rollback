#include <engine/engine.h>

#include <SFML/Window/Event.hpp>

namespace core
{
void Engine::Run()
{
    Init();
    sf::Clock clock;
    while (window_->isOpen())
    {
        const auto dt = clock.restart();
        Update(dt);
    }
    Destroy();
}

void Engine::Init()
{
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(1230, 720), "Rollback Game");
}

void Engine::Update(sf::Time time)
{
    sf::Event e{};
    while (window_->pollEvent(e))
    {
        switch (e.type)
        {
        case sf::Event::Closed:
            window_->close();
            break;
        default:
            break;
        }
    }
}

void Engine::Destroy()
{
    window_ = nullptr;
}
} // namespace core
