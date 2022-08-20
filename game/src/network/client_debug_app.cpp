#include <network/client_debug_app.h>

#include "game/input_manager.h"

namespace game
{
    NetworkDebugApp::NetworkDebugApp()
    {
    }

    void NetworkDebugApp::Init()
    {
        windowSize_ = core::windowSize;
        for (auto& framebuffer : clientsFramebuffers_)
        {
            framebuffer.create(windowSize_.x / 2u, windowSize_.y);
        }
        for (auto& client : clients_)
        {
            client.SetWindowSize(sf::Vector2u(windowSize_.x / 2u, windowSize_.y));
            client.Init();
        }
    }

    void NetworkDebugApp::Update(sf::Time dt)
    {
        for(std::size_t i = 0; i < clients_.size(); i++)
        {
            clients_[i].SetPlayerInput(GetPlayerInput(static_cast<int>(i)));
        }

        for (auto& client : clients_)
        {
            client.Update(dt);
        }
    }

    void NetworkDebugApp::Destroy()
    {
        for (auto& client : clients_)
        {
            client.Destroy();
        }
        
    }

    void NetworkDebugApp::DrawImGui()
    {
        for (auto& client : clients_)
        {
            client.DrawImGui();
        }
    }

    void NetworkDebugApp::Draw(sf::RenderTarget& renderTarget)
    {
        for (PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            clientsFramebuffers_[playerNumber].clear(sf::Color::Black);
            clients_[playerNumber].Draw(clientsFramebuffers_[playerNumber]);
            clientsFramebuffers_[playerNumber].display();
        }
        screenQuad_ = sf::Sprite();
        screenQuad_.setTexture(clientsFramebuffers_[0].getTexture());
        renderTarget.draw(screenQuad_);

        screenQuad_.setTexture(clientsFramebuffers_[1].getTexture());
        screenQuad_.setPosition(sf::Vector2f(static_cast<float>(windowSize_.x / 2u), 0));
        renderTarget.draw(screenQuad_);
    }

    void NetworkDebugApp::OnEvent(const sf::Event& event)
    {
        switch (event.type)
        {
        case sf::Event::Resized:
        {
            windowSize_ = sf::Vector2u(event.size.width, event.size.height);
            for (auto& framebuffer : clientsFramebuffers_)
            {
                framebuffer.create(windowSize_.x / 2u, windowSize_.y);
            }

            for (auto& client : clients_)
            {
                client.SetWindowSize(sf::Vector2u(windowSize_.x / 2u, windowSize_.y));
            }

            break;
        }
        default:;
        }
    }
}
