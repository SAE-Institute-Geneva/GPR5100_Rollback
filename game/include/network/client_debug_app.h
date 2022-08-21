#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "client.h"
#include "network_client.h"
#include "network/app.h"
#include "game/game_globals.h"


namespace game
{

class NetworkDebugApp final : public App
{
public:
    NetworkDebugApp();

    void Init() override;

    void Update(sf::Time dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Draw(sf::RenderTarget& renderTarget) override;

    void OnEvent(const sf::Event& event) override;
private:
    std::array<NetworkClient, maxPlayerNmb> clients_;
    std::array<sf::RenderTexture, maxPlayerNmb> clientsFramebuffers_;
    sf::Sprite screenQuad_;
    sf::Vector2u windowSize_;
};


}
