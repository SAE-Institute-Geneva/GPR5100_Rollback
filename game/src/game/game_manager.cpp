#include <game/game_manager.h>

#include "utils/log.h"
#include <fmt/format.h>
#include <imgui.h>

#include "utils/conversion.h"

namespace game
{

    GameManager::GameManager() :
        transformManager_(entityManager_),
        rollbackManager_(*this, entityManager_)
    {
    }

    void GameManager::Init()
    {
        entityMap_.fill(core::EntityManager::INVALID_ENTITY);
    }

    void GameManager::Update(sf::Time dt)
    {
    }

    void GameManager::Destroy()
    {

    }

    void GameManager::SpawnPlayer(PlayerNumber playerNumber, core::Vec2f position, core::degree_t rotation)
    {
        if (GetEntityFromPlayerNumber(playerNumber) != core::EntityManager::INVALID_ENTITY)
            return;
        core::LogDebug("[GameManager] Spawning new player");
        const auto entity = entityManager_.CreateEntity();
        entityMap_[playerNumber] = entity;
        
        entityManager_.AddComponent(entity, static_cast<core::EntityMask>(ComponentType::PLAYER_CHARACTER));
        transformManager_.AddComponent(entity);
        transformManager_.SetPosition(entity, position);
        transformManager_.SetRotation(entity, rotation);
        rollbackManager_.SpawnPlayer(playerNumber, entity, position, core::degree_t(rotation));
    }

    core::Entity GameManager::GetEntityFromPlayerNumber(PlayerNumber playerNumber) const
    {
        return entityMap_[playerNumber];
    }


    void GameManager::SetPlayerInput(PlayerNumber playerNumber, PlayerInput playerInput, std::uint32_t inputFrame)
    {
        if (playerNumber == INVALID_PLAYER)
            return;

        rollbackManager_.SetPlayerInput(playerNumber, playerInput, inputFrame);

    }
    void GameManager::Validate(Frame newValidateFrame)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Validate Frame");
#endif
        if (rollbackManager_.GetCurrentFrame() < newValidateFrame)
        {
            rollbackManager_.StartNewFrame(newValidateFrame);
        }
        rollbackManager_.ValidateFrame(newValidateFrame);
    }

    core::Entity GameManager::SpawnBullet(PlayerNumber playerNumber, core::Vec2f position, core::Vec2f velocity)
    {
        const core::Entity entity = entityManager_.CreateEntity();
        entityManager_.AddComponent(entity, static_cast<core::EntityMask>(ComponentType::BULLET));
        transformManager_.AddComponent(entity);
        transformManager_.SetPosition(entity, position);
        transformManager_.SetScale(entity, core::Vec2f::one() * bulletScale);
        transformManager_.SetRotation(entity, core::degree_t(0.0f));
        rollbackManager_.SpawnBullet(playerNumber, entity, position, velocity);
        return entity;
    }

    void GameManager::DestroyBullet(core::Entity entity)
    {
        rollbackManager_.DestroyEntity(entity);
    }

    PlayerNumber GameManager::CheckWinner() const
    {
        int alivePlayer = 0;
        PlayerNumber winner = INVALID_PLAYER;
        const auto& playerManager = rollbackManager_.GetPlayerCharacterManager();
        for (core::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
        {
            if (!entityManager_.HasComponent(entity, static_cast<core::EntityMask>(ComponentType::PLAYER_CHARACTER)))
                continue;
            const auto& player = playerManager.GetComponent(entity);
            if (player.health > 0)
            {
                alivePlayer++;
                winner = player.playerNumber;
            }
        }

        return alivePlayer == 1 ? winner : INVALID_PLAYER;
    }

    void GameManager::WinGame(PlayerNumber winner)
    {
        winner_ = winner;
    }

    ClientGameManager::ClientGameManager(PacketSenderInterface& packetSenderInterface) :
        GameManager(),
        spriteManager_(entityManager_, transformManager_),
        packetSenderInterface_(packetSenderInterface)
    {
    }

    void ClientGameManager::Init()
    {
        //TODO setup camera with render area dimension

        //TODO load textures

        //TODO load fonts


        GameManager::Init();
    }

    void ClientGameManager::Update(sf::Time dt)
    {
        if (state_ & STARTED)
        {
            rollbackManager_.SimulateToCurrentFrame();
            //Copy rollback transform position to our own
            for (core::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
            {
                if (entityManager_.HasComponent(entity,
                                                core::EntityMask(ComponentType::PLAYER_CHARACTER) |
                                                core::EntityMask(core::ComponentType::SPRITE)))
                {
                    const auto& player = rollbackManager_.GetPlayerCharacterManager().GetComponent(entity);
                    auto sprite = spriteManager_.GetComponent(entity);
                    if (player.invincibilityTime > 0.0f)
                    {
                        auto leftV = std::fmod(player.invincibilityTime, invincibilityFlashPeriod);
                        auto rightV = invincibilityFlashPeriod / 2.0f;
                        //core::LogDebug(fmt::format("Comparing {} and {} with time: {}", leftV, rightV, player.invincibilityTime));
                    }
                    if (player.invincibilityTime > 0.0f &&
                        std::fmod(player.invincibilityTime, invincibilityFlashPeriod)
                    > invincibilityFlashPeriod / 2.0f)
                    {
                        sprite.setColor(sf::Color::Black);
                    }
                    else
                    {
                        sprite.setColor(playerColors[player.playerNumber]);
                    }
                    spriteManager_.SetComponent(entity, sprite);
                }

                if (entityManager_.HasComponent(entity, core::EntityMask(core::ComponentType::TRANSFORM)))
                {
                    transformManager_.SetPosition(entity, rollbackManager_.GetTransformManager().GetPosition(entity));
                    transformManager_.SetScale(entity, rollbackManager_.GetTransformManager().GetScale(entity));
                    transformManager_.SetRotation(entity, rollbackManager_.GetTransformManager().GetRotation(entity));
                }
            }
        }
        fixedTimer_ += dt.asSeconds();
        while (fixedTimer_ > FixedPeriod)
        {
            FixedUpdate();
            fixedTimer_ -= FixedPeriod;

        }


        
    }

    void ClientGameManager::Destroy()
    {
        GameManager::Destroy();
    }

    void ClientGameManager::SetWindowSize(sf::Vector2u windowsSize)
    {
        windowSize_ = windowsSize;
        camera_.setSize(windowsSize.x, windowsSize.y);
    }

    void ClientGameManager::Draw(sf::RenderTarget& window)
    {
        window.setView(camera_);
        spriteManager_.Draw(window);
        // TODO Draw texts on screen
        /*
        if (state_ & FINISHED)
        {
            if (winner_ == GetPlayerNumber())
            {
                const std::string winnerText = fmt::format("You won!");
                fontManager_.RenderText(fontId_, winnerText, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
            }
            else if (winner_ != INVALID_PLAYER)
            {
                const std::string winnerText = fmt::format("P{} won!", winner_ + 1);
                fontManager_.RenderText(fontId_, winnerText, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
            }
            else
            {
                const std::string errorMessage = fmt::format("Error with other players");
                fontManager_.RenderText(fontId_, errorMessage, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
            }
        }
        if (!(state_ & STARTED))
        {
            if (startingTime_ != 0)
            {
                using namespace std::chrono;
                unsigned long long ms = duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch()
                    ).count();
                if (ms < startingTime_)
                {
                    const std::string countDownText = fmt::format("Starts in {}", ((startingTime_ - ms) / 1000 + 1));
                    fontManager_.RenderText(fontId_, countDownText, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
                }
            }
        }
        else
        {
            std::string health;
            const auto& playerManager = rollbackManager_.GetPlayerCharacterManager();
            for (PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
            {
                const auto playerEntity = GetEntityFromPlayerNumber(playerNumber);
                health += fmt::format("P{} health: {} ", playerNumber + 1, playerManager.GetComponent(playerEntity).health);
            }
            fontManager_.RenderText(fontId_, health, Vec2f(0.0f, -40.0f), TextAnchor::TOP_LEFT, 0.75f, Color4(Color::white, 1.0f));
        }
        */
    }

    void ClientGameManager::SpawnPlayer(PlayerNumber playerNumber, core::Vec2f position, core::degree_t rotation)
    {
        core::LogDebug("Spawn player: " + std::to_string(playerNumber));
        GameManager::SpawnPlayer(playerNumber, position, rotation);
        const auto entity = GetEntityFromPlayerNumber(playerNumber);
        spriteManager_.AddComponent(entity);
        spriteManager_.SetTexture(entity, shipTexture_);
        auto sprite = spriteManager_.GetComponent(entity);
        sprite.setColor(playerColors[playerNumber]);
        spriteManager_.SetComponent(entity, sprite);

    }

    core::Entity ClientGameManager::SpawnBullet(PlayerNumber playerNumber, core::Vec2f position, core::Vec2f velocity)
    {
        const auto entity = GameManager::SpawnBullet(playerNumber, position, velocity);

        spriteManager_.AddComponent(entity);
        spriteManager_.SetTexture(entity, bulletTexture_);
        auto sprite = spriteManager_.GetComponent(entity);
        sprite.setColor(playerColors[playerNumber]);
        spriteManager_.SetComponent(entity, sprite);
        return entity;
    }


    void ClientGameManager::FixedUpdate()
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Game Manager Fixed Update");
#endif
        if (!(state_ & STARTED))
        {
            if (startingTime_ != 0)
            {
                using namespace std::chrono;
                unsigned long long ms = duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch()
                    ).count();
                if (ms > startingTime_)
                {
                    state_ = state_ | STARTED;
                }
                else
                {

                    return;
                }
            }
            else
            {
                return;
            }
        }
        if (state_ & FINISHED)
        {
            return;
        }

        //We send the player inputs when the game started

        const auto& inputs = rollbackManager_.GetInputs(GetPlayerNumber());
        auto playerInputPacket = std::make_unique<PlayerInputPacket>();
        playerInputPacket->playerNumber = GetPlayerNumber();
        playerInputPacket->currentFrame = core::ConvertToBinary(currentFrame_);
        for (size_t i = 0; i < playerInputPacket->inputs.size(); i++)
        {
            if (i > currentFrame_)
            {
                break;
            }

            playerInputPacket->inputs[i] = inputs[i];
        }
        packetSenderInterface_.SendUnreliablePacket(std::move(playerInputPacket));


        currentFrame_++;
        rollbackManager_.StartNewFrame(currentFrame_);
    }


    void ClientGameManager::SetPlayerInput(PlayerNumber playerNumber, PlayerInput playerInput, std::uint32_t inputFrame)
    {
        if (playerNumber == INVALID_PLAYER)
            return;
        GameManager::SetPlayerInput(playerNumber, playerInput, inputFrame);
    }

    void ClientGameManager::StartGame(unsigned long long int startingTime)
    {
        core::LogDebug("Start game at starting time: " + std::to_string(startingTime));
        startingTime_ = startingTime;
    }

    void ClientGameManager::DrawImGui()
    {
        ImGui::Text(state_ & STARTED ? "Game has started" : "Game has not started");
        if (startingTime_ != 0)
        {
            ImGui::Text("Starting Time: %llu", startingTime_);
            using namespace std::chrono;
            unsigned long long ms = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()
                ).count();
            ImGui::Text("Current Time: %llu", ms);
        }
    }

    void ClientGameManager::ConfirmValidateFrame(Frame newValidateFrame,
        const std::array<PhysicsState, maxPlayerNmb>& physicsStates)
    {
        if (newValidateFrame < rollbackManager_.GetLastValidateFrame())
        {
            //core::LogDebug(fmt::format("[Warning] New validate frame is too old"));
            return;
        }
        for (PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            if (rollbackManager_.GetLastReceivedFrame(playerNumber) < newValidateFrame)
            {
                /*
                core::LogDebug(fmt::format("[Warning] Trying to validate frame {} while playerNumber {} is at input frame {}, client player {}",
                    newValidateFrame,
                    playerNumber + 1,
                    rollbackManager_.GetLastReceivedFrame(playerNumber),
                    GetPlayerNumber()+1));
                */
                return;
            }
        }
        rollbackManager_.ConfirmFrame(newValidateFrame, physicsStates);
    }

    void ClientGameManager::WinGame(PlayerNumber winner)
    {
        GameManager::WinGame(winner);
        state_ = state_ | FINISHED;
    }

}
