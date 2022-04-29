#pragma once
#include <client/object/level/core/level.h>
#include "object/map/map_loader.h"
namespace revive
{
	using namespace client_fw;

	class GamePlayLevel final : public Level
	{
	public:
		GamePlayLevel();
		virtual ~GamePlayLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	private:
		void GenerateVisualActors();

	private:
		virtual UPtr<GameMode> CreateGameMode() const override;

		std::vector<SPtr<Actor>> m_actors;

		MapLoader m_map_loader;
	public:
		virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;
		virtual std::vector<SPtr<CollisionOctree>> CreateCollisionOctrees() const override;

	private:
		bool m_is_succeed_login = false;
	};
}