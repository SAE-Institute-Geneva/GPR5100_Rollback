#include "network/client_app.h"

#include "engine/globals.h"
#include "game/input_manager.h"

namespace game
{
void ClientApp::Begin()
{
    windowSize_ = core::windowSize;
    client_.SetWindowSize(windowSize_);
    client_.Begin();
}

void ClientApp::Update(sf::Time dt)
{
    client_.SetPlayerInput(GetPlayerInput(0));
    client_.Update(dt);
}

void ClientApp::End()
{
    client_.End();
}

void ClientApp::DrawImGui()
{
    client_.DrawImGui();
}

void ClientApp::OnEvent(const sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::Resized:
    {
        windowSize_ = sf::Vector2u(event.size.width, event.size.height);
        client_.SetWindowSize(windowSize_);
        break;
    }
    default:;
    }
}
} // namespace game
