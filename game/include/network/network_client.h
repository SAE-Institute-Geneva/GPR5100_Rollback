#pragma once
#include "client.h"
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>

namespace game
{
/**
 * \brief NetworkClient is a network client that uses SFML sockets.
 */
class NetworkClient final : public Client
{
public:
	enum class State
	{
		NONE,
		JOINING,
		JOINED,
		GAME_STARTING,
		GAME

	};
	enum class PacketSource
	{
		TCP,
		UDP
	};
	void Begin() override;

	void Update(sf::Time dt) override;

	void End() override;

	void DrawImGui() override;

	void Draw(sf::RenderTarget& renderTarget) override;

	void SendReliablePacket(std::unique_ptr<Packet> packet) override;

	void SendUnreliablePacket(std::unique_ptr<Packet> packet) override;
	void SetPlayerInput(PlayerInput input);


private:
	void ReceivePacket(sf::Packet& packet, PacketSource source);
	sf::UdpSocket udpSocket_;
	sf::TcpSocket tcpSocket_;

	std::string serverAddress_ = "localhost";
	unsigned short serverTcpPort_ = 12345;
	unsigned short serverUdpPort_ = 0;


	State currentState_ = State::NONE;
};
}
