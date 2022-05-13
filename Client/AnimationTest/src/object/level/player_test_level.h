#pragma once
#include "client/object/level/core/level.h"

namespace anim_test
{
	using namespace client_fw;

	class PlayerTestLevel final : public Level
	{
	public:
		PlayerTestLevel();
		virtual ~PlayerTestLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

	private:
		int m_animation_set_index = 0;
		SPtr<class DefaultPawn> m_player;
		SPtr<class PlayerController> m_player_controller;
	public:
		SPtr<class TestActor> test_mesh;

		virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;
		virtual std::vector<SPtr<CollisionOctree>> CreateCollisionOctrees() const override;
	};

}

