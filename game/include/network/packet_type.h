/**
 * \file packet_type.h
 */
#pragma once

#include <SFML/Network/Packet.hpp>

#include "game/game_globals.h"
#include <memory>
#include <chrono>

namespace game
{
enum class PacketType : std::uint8_t
{
    JOIN = 0u,
    SPAWN_PLAYER,
    INPUT,
    VALIDATE_STATE,
    START_GAME,
    JOIN_ACK,
    WIN_GAME,
    PING,
    NONE,
};

/**
 * \brief PhysicsState is the type of the physics state checksum
 */
using PhysicsState = std::uint16_t;



template<class T>
concept Loadable = requires(const T& t, sf::Packet& p)
{
    { t.Load(p) } -> std::same_as<sf::Packet&>;
};

template<class T>
concept Savable = requires(T& t, sf::Packet & p)
{
    { t.Save(p) } -> std::same_as<sf::Packet&>;
};


template<Loadable T>
sf::Packet& operator<<(sf::Packet& packetReceived, const T& packet)
{
    return packet.Load(packetReceived);
}

template<Savable T>
sf::Packet& operator>>(sf::Packet& packetReceived, T& packet)
{
    return packet.Save(packetReceived);
}

template<Savable T>
std::unique_ptr<T> Generate(sf::Packet& p)
{
    auto packet = std::make_unique<T>();
    packet->Save(p);
    return packet;
}




template<typename T, size_t N>
sf::Packet& operator<<(sf::Packet& packet, const std::array<T, N>& t)
{
    for (auto& tmp : t)
    {
        packet << tmp;
    }
    return packet;
}

template <typename T, size_t N>
sf::Packet& operator>>(sf::Packet& packet, std::array<T, N>& t)
{
    for (auto& tmp : t)
    {
        packet >> tmp;
    }
    return packet;
}

/**
 * \brief Packet is a interface that defines what a packet with a PacketType.
 */
struct Packet
{
    virtual ~Packet() = default;
    PacketType packetType = PacketType::NONE;
    virtual sf::Packet& Load(sf::Packet& p) const
    {
        const auto type = static_cast<std::uint8_t>(packetType);
        p << type;
        return p;
    }

    virtual sf::Packet& Save(sf::Packet& p)
    {
        std::uint8_t type;
        p >> type;
        packetType = static_cast<PacketType>(type);
        return p;
    }
};

template<PacketType type>
std::unique_ptr <Packet> GenerateReceivedPacket(sf::Packet& p);

/**
 * \brief TypedPacket is a template class that sets the packetType of Packet automatically at construction with the given type.
 * \tparam type is the PacketType of the packet
 */
template<PacketType type>
struct TypedPacket : Packet
{
    TypedPacket() { packetType = type; }
};


/**
 * \brief JoinPacket is a TCP Packet that is sent by a client to the server to join a game.
 */
struct JoinPacket : TypedPacket<PacketType::JOIN>
{
    std::array<std::uint8_t, sizeof(ClientId)> clientId{};
    std::array<std::uint8_t, sizeof(unsigned long)> startTime{};
    sf::Packet& Load(sf::Packet& p) const override
    {

        return p << clientId << startTime;
    }

    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> clientId >> startTime;
    }
};

template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::JOIN>(sf::Packet& p)
{
    return Generate<JoinPacket>(p);
}

/**
 * \brief JoinAckPacket is a TCP Packet that is sent by the server to the client to answer a join packet
 */
struct JoinAckPacket : TypedPacket<PacketType::JOIN_ACK>
{
    std::array<std::uint8_t, sizeof(ClientId)> clientId{};
    std::array<std::uint8_t, sizeof(unsigned short)> udpPort{};

    sf::Packet& Load(sf::Packet& p) const override
    {
        return p << clientId << udpPort;
    }

    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> clientId >> udpPort;
    }
};
template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::JOIN_ACK>(sf::Packet& p)
{
    return Generate<JoinAckPacket>(p);
}

/**
 * \brief SpawnPlayerPacket is a TCP Packet sent by the server to all clients to notify of the spawn of a new player
 */
struct SpawnPlayerPacket : TypedPacket<PacketType::SPAWN_PLAYER>
{
    std::array<std::uint8_t, sizeof(ClientId)> clientId{};
    PlayerNumber playerNumber = INVALID_PLAYER;
    std::array<std::uint8_t, sizeof(core::Vec2f)> pos{};
    std::array<std::uint8_t, sizeof(core::Degree)> angle{};

    sf::Packet& Load(sf::Packet& p) const override
    {
        return p << clientId << playerNumber <<
            pos << angle;
    }

    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> clientId >> playerNumber >>
            pos >> angle;
    }
};

template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::SPAWN_PLAYER>(sf::Packet& p)
{
    return Generate<SpawnPlayerPacket>(p);
}
    
/**
 * \brief PlayerInputPacket is a UDP Packet sent by the player client and then replicated by the server to all clients to share the currentFrame
 * and all the previous ones player inputs.
 */
struct PlayerInputPacket : TypedPacket<PacketType::INPUT>
{
    PlayerNumber playerNumber = INVALID_PLAYER;
    std::array<std::uint8_t, sizeof(Frame)> currentFrame{};
    std::array<PlayerInput, maxInputNmb> inputs{};

    sf::Packet& Load(sf::Packet& p) const override
    {
        return p << playerNumber <<
            currentFrame << inputs;
    }

    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> playerNumber >>
            currentFrame >> inputs;
    }
};
template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::INPUT>(sf::Packet& p)
{
    return Generate<PlayerInputPacket>(p);
}

/**
 * \brief StartGamePacket is a TCP Packet send by the server to start a game at a given time.
 */
struct StartGamePacket : TypedPacket<PacketType::START_GAME>
{
};

template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::START_GAME>(sf::Packet& p)
{
    return Generate<StartGamePacket>(p);
}

/**
 * \brief ValidateFramePacket is an UDP packet that is sent by the server to validate the last physics state of the world.
 */
struct ValidateFramePacket : TypedPacket<PacketType::VALIDATE_STATE>
{
    std::array<std::uint8_t, sizeof(Frame)> newValidateFrame{};
    std::array<std::uint8_t, sizeof(PhysicsState) * maxPlayerNmb> physicsState{};

    sf::Packet& Load(sf::Packet& p) const override
    {
        return p << newValidateFrame << physicsState;
    }

    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> newValidateFrame >> physicsState;
    }
};
template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::VALIDATE_STATE>(sf::Packet& p)
{
    return Generate<ValidateFramePacket>(p);
}

/**
 * \brief WinGamePacket is a TCP Packet sent by the server to notify the clients that a certain player has won.
 */
struct WinGamePacket : TypedPacket<PacketType::WIN_GAME>
{
    PlayerNumber winner = INVALID_PLAYER;
    sf::Packet& Load(sf::Packet& p) const override
    {
        return p << winner;
    }
    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> winner;
    }
};

template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::WIN_GAME>(sf::Packet& p)
{
    return Generate<WinGamePacket>(p);
}
/**
 * \brief PingPacket is an UDP Packet sent by the client to the server and resend by the server to measure the RTT between the client and the server.
 */
struct PingPacket : TypedPacket<PacketType::PING>
{
    std::array<std::uint8_t, sizeof(unsigned long long)> time{};
    std::array<std::uint8_t, sizeof(ClientId)> clientId{};
    sf::Packet& Load(sf::Packet& p) const override
    {
        return p << time << clientId;
    }
    sf::Packet& Save(sf::Packet& p) override
    {
        return p >> time >> clientId;
    }
};

template<>
inline std::unique_ptr <Packet> GenerateReceivedPacket<PacketType::PING>(sf::Packet& p)
{
    return Generate< PingPacket>(p);
}

inline void GenerateSendingPacket(sf::Packet& packet, const Packet& sendingPacket)
{
    packet << sendingPacket;
    sendingPacket.Load(packet);
}

template<PacketType type>
std::unique_ptr<Packet> GenerateFromType(PacketType t, sf::Packet& packet)
{
    if constexpr (type == PacketType::NONE)
        return nullptr;
    if(type == t)
    {
        return GenerateReceivedPacket<type>(packet);
    }
    return GenerateFromType< static_cast<PacketType>(static_cast<int>(type) + 1)>(t, packet);
}

inline std::unique_ptr<Packet> GenerateReceivedPacket(sf::Packet& packet)
{
    Packet packetTmp;
    packet >> packetTmp;
    return GenerateFromType<static_cast<PacketType>(0)>(packetTmp.packetType, packet);
}

/**
 * \brief PacketSenderInterface is a interface for any Server or Client who wants to send and receive packets
 */
class PacketSenderInterface
{
public:
    virtual ~PacketSenderInterface() = default;
    virtual void SendReliablePacket(std::unique_ptr<Packet> packet) = 0;
    virtual void SendUnreliablePacket(std::unique_ptr<Packet> packet) = 0;
};
}
