#pragma once
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "game_globals.h"
#include "engine/entity.h"
#include "graphics/graphics.h"
#include "graphics/sprite.h"
#include "engine/system.h"
#include "engine/transform.h"
#include "network/packet_type.h"

namespace game
{
class PacketSenderInterface;

class GameManager : public core::SystemInterface
	{
	public:
		GameManager();

		void Init() override;
		void Update(sf::Time dt) override;
		void Destroy() override;
		virtual void SpawnPlayer(PlayerNumber playerNumber, core::Vec2f position, core::degree_t rotation);
		virtual core::Entity SpawnBullet(PlayerNumber, core::Vec2f position, core::Vec2f velocity);
		virtual void DestroyBullet(core::Entity entity);
		[[nodiscard]] core::Entity GetEntityFromPlayerNumber(PlayerNumber playerNumber) const;
		[[nodiscard]] Frame GetCurrentFrame() const { return currentFrame_; }
		[[nodiscard]] Frame GetLastValidateFrame() const { return rollbackManager_.GetLastValidateFrame(); }
		[[nodiscard]] const core::TransformManager& GetTransformManager() const { return transformManager_; }
		[[nodiscard]] const RollbackManager& GetRollbackManager() const { return rollbackManager_; }
		virtual void SetPlayerInput(PlayerNumber playerNumber, std::uint8_t playerInput, std::uint32_t inputFrame);
		/*
		 * \brief Called by the server to validate a frame
		 */
		void Validate(Frame newValidateFrame);
		static constexpr float PixelPerUnit = 100.0f;
		static constexpr float FixedPeriod = 0.02f; //50fps
		PlayerNumber CheckWinner() const;
		virtual void WinGame(PlayerNumber winner);
	protected:
        core::EntityManager entityManager_;
        core::TransformManager transformManager_;
		RollbackManager rollbackManager_;
		std::array<core::Entity, maxPlayerNmb> entityMap_{};
		Frame currentFrame_ = 0;
		PlayerNumber winner_ = INVALID_PLAYER;
	};

	class ClientGameManager : public GameManager,
		public core::DrawInterface, public core::DrawImGuiInterface
	{
	public:
		enum State : std::uint32_t
		{
			STARTED = 1u << 0u,
			FINISHED = 1u << 1u,
		};
		explicit ClientGameManager(PacketSenderInterface& packetSenderInterface);
		void StartGame(unsigned long long int startingTime);
		void Init() override;
		void Update(sf::Time dt) override;
		void Destroy() override;
		void SetWindowSize(sf::Vector2u windowsSize);
		[[nodiscard]] sf::Vector2u GetWindowSize() const { return windowSize_; }
		void Render() override;
		void SetClientPlayer(PlayerNumber clientPlayer) { clientPlayer_ = clientPlayer; }
		[[nodiscard]] const Camera2D& GetCamera() const { return camera_; }
		void SpawnPlayer(PlayerNumber playerNumber, core::Vec2f position, core::degree_t rotation) override;
        core::Entity SpawnBullet(PlayerNumber playerNumber, core::Vec2f position, core::Vec2f velocity) override;
		void FixedUpdate();
		void SetPlayerInput(PlayerNumber playerNumber, std::uint8_t playerInput, std::uint32_t inputFrame) override;
		void DrawImGui() override;
		void ConfirmValidateFrame(Frame newValidateFrame, const std::array<PhysicsState, maxPlayerNmb>& physicsStates);
		[[nodiscard]] PlayerNumber GetPlayerNumber() const { return clientPlayer_; }
		void WinGame(PlayerNumber winner) override;
		[[nodiscard]] std::uint32_t GetState() const { return state_; }
	protected:
		PacketSenderInterface& packetSenderInterface_;
		sf::Vector2u windowSize_;
		Camera2D camera_;
		PlayerNumber clientPlayer_ = INVALID_PLAYER;
		core::SpriteManager spriteManager_;
		float fixedTimer_ = 0.0f;
		unsigned long long startingTime_ = 0;
		std::uint32_t state_ = 0;

		sf::Texture shipTexture_;
		sf::Texture bulletTexture_;
		sf::Font font_;
	};
}
