#include <network/full_debug_app.h>

#include "engine/globals.h"
#include "game/input_manager.h"

namespace game
{
    SimulationDebugApp::SimulationDebugApp() : server_(clients_)
    {
        for (auto& client : clients_)
        {
            client = std::make_unique<SimulationClient>(server_);
        }
    }

    void SimulationDebugApp::OnEvent(const sf::Event& event)
    {
        switch(event.type)
        {
        case sf::Event::Resized:
        {
            windowSize_ = sf::Vector2u(event.size.width, event.size.height);
            for (auto& framebuffer : clientsFramebuffers_)
            {
                framebuffer.create(windowSize_.x/2u, windowSize_.y);
            }

            for (auto& client : clients_)
            {
                client->SetWindowSize(sf::Vector2u(windowSize_.x/2u, windowSize_.y));
            }

            break;
        }
        default: ;
        }
    }

    void SimulationDebugApp::Init()
    {
        windowSize_ = core::windowSize;
        for(auto& framebuffer: clientsFramebuffers_)
        {
            framebuffer.create(windowSize_.x / 2u, windowSize_.y);
        }
        for(auto& client : clients_)
        {
            client->SetWindowSize(sf::Vector2u(windowSize_.x / 2u, windowSize_.y));
            client->Init();
        }
        server_.Init();
        
    }

    void SimulationDebugApp::Update(sf::Time dt)
    {
        
        //Checking if keys are down
        for(std::size_t i = 0; i < clients_.size(); i++)
        {
            clients_[i]->SetPlayerInput(GetPlayerInput(static_cast<int>(i)));
        }
        
        
        server_.Update(dt);
        for (const auto& client : clients_)
        {
            client->Update(dt);
        }
    }

    void SimulationDebugApp::Destroy()
    {
        for (const auto& client : clients_)
        {
            client->Destroy();
        }
        server_.Destroy();
    }

    void SimulationDebugApp::DrawImGui()
    {
        server_.DrawImGui();
        for (auto& client : clients_)
        {
            client->DrawImGui();
        }
    }

    void SimulationDebugApp::Draw(sf::RenderTarget& renderTarget)
    {
        for (PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            clientsFramebuffers_[playerNumber].clear(sf::Color::Black);
            clients_[playerNumber]->Draw(clientsFramebuffers_[playerNumber]);
            clientsFramebuffers_[playerNumber].display();
        }
        screenQuad_ = sf::Sprite();
        screenQuad_.setTexture(clientsFramebuffers_[0].getTexture());
        renderTarget.draw(screenQuad_);

        screenQuad_.setTexture(clientsFramebuffers_[1].getTexture());
        screenQuad_.setPosition(sf::Vector2f(static_cast<float>(windowSize_.x / 2u), 0.0f));
        renderTarget.draw(screenQuad_);

    }
}
