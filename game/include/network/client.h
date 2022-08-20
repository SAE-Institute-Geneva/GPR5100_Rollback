#pragma once
#include "packet_type.h"
#include "game/game_manager.h"
#include "graphics/graphics.h"

namespace game
{
/**
 * \brief Client is an interface of a player game manager and the net client interface (receive and send packets).
 * A client needs an ID which is receive by the server through a packet.
 */
class Client : public core::DrawInterface, public core::DrawImGuiInterface, public PacketSenderInterface, public core::SystemInterface
    {
    public:
        Client() : gameManager_(*this)
        {

        }
        virtual ~Client() = default;
        virtual void SetWindowSize(sf::Vector2u windowSize)
        {
            gameManager_.SetWindowSize(windowSize);
        }

        /**
         * \brief ReceivePacket is a method called by an app owning a client when receiving a packet.
         * It is the same one for simulated and network client
         * \param packet A non-owning pointer to a packet (you don't need to care about deleting it
         */
        virtual void ReceivePacket(const Packet* packet);
    protected:

        ClientGameManager gameManager_;
        ClientId clientId_ = 0;
    };
}
