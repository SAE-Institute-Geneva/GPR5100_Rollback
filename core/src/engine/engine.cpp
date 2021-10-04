#include <engine/engine.h>

#include <SFML/Window/Event.hpp>

#include "engine/system.h"
#include "graphics/graphics.h"

#include <imgui.h>
#include <imgui-SFML.h>

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

void Engine::RegisterSystem(SystemInterface* system)
{
    systems_.push_back(system);
}

void Engine::RegisterOnEvent(OnEventInterface* onEventInterface)
{
    eventInterfaces_.push_back(onEventInterface);
}

void Engine::RegisterDraw(DrawInterface* drawInterface)
{
    drawInterfaces_.push_back(drawInterface);
}

void Engine::RegisterDrawImGui(DrawImGuiInterface* drawImGuiInterface)
{
    drawImGuiInterfaces_.push_back(drawImGuiInterface);
}

void Engine::Init()
{
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(1230, 720), "Rollback Game");
    ImGui::SFML::Init(*window_);
}

void Engine::Update(sf::Time dt)
{
    sf::Event e{};
    while (window_->pollEvent(e))
    {
        ImGui::SFML::ProcessEvent(e);
        switch (e.type)
        {
        case sf::Event::Closed:
            window_->close();
            break;
        default:
            break;
        }
        for(auto* eventInterface : eventInterfaces_)
        {
            eventInterface->OnEvent(e);
        }
    }
    for(auto* system : systems_)
    {
        system->Update(dt);
    }
    ImGui::SFML::Update(*window_, dt);
    window_->clear(sf::Color::Black);

    for(auto* drawInterface : drawInterfaces_)
    {
        drawInterface->Draw(*window_);
    }
    for(auto* drawImGuiInterface : drawImGuiInterfaces_)
    {
        drawImGuiInterface->DrawImGui();
    }
    ImGui::SFML::Render(*window_);

    window_->display();
}

void Engine::Destroy()
{
    window_ = nullptr;
}
} // namespace core
