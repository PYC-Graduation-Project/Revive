#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/input/input.h>
#include <client/renderer/core/render.h>
#include <client/renderer/core/render_system.h>
#include <client/renderer/renderlevel/core/render_level.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include "object/level/player_test_level.h"
#include "object/actor/test_actor.h"

namespace anim_test
{
	PlayerTestLevel::PlayerTestLevel()
		: Level("player test level")
	{
	}

	bool PlayerTestLevel::Initialize()
	{
		m_player = CreateSPtr<DefaultPawn>();
		m_player_controller = CreateSPtr<PlayerController>();

		m_player_controller->Possess(m_player);

		SpawnActor(m_player);
		SpawnActor(m_player_controller);

		auto violet_idle = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Contents/Revive/violet.rev","idle");
		auto skel_run = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Contents/Revive/skel.rev","run");
		SpawnActor(violet_idle);
		SpawnActor(skel_run);
		violet_idle->SetPosition(Vec3{ 10.0f, -35.0f, 1100.0f });
		violet_idle->SetScale(Vec3(0.5f,0.5f,0.5f));
		violet_idle->SetRotation(80.0f, 0.0f, 0.0f);
		skel_run->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });
		skel_run->SetScale(Vec3(50.0f, 50.0f, 50.0f));
		skel_run->SetRotation(80.0f, 0.0f, 0.0f);
		

		Input::SetInputMode(eInputMode::kUIAndGame);
		Input::SetHideCursor(true);

		return true;
	}

	void PlayerTestLevel::Shutdown()
	{
		Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void PlayerTestLevel::Update(float delta_time)
	{
		
		static float x = -300.0f, y = 0.0f, z = 1100.0f;
		static float time = 0.0f;

		static UINT count = 1;

		time += delta_time;

		if (time >= 0.016f && count <= 1000)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (i % 2 == 0)
				{
					auto police = CreateSPtr<TestActor>(eMobilityState::kDestructable, "../Contents/Revive/violet.rev","idle");
					SpawnActor(police);
					police->SetPosition(Vec3{ x, y- 35.0f, z });
					police->SetScale(Vec3(0.5f, 0.5f, 0.5f));
					police->SetRotation(80.0f, 0.0f, 0.0f);
					++count;
				}
				else
				{
					auto police = CreateSPtr<TestActor>(eMobilityState::kDestructable, "../Contents/Revive/violet.rev","idle");
					SpawnActor(police);
					police->SetPosition(Vec3{ x, y, z });
					police->SetScale(50.f);
					police->SetPosition(Vec3{ x, y - 35.0f, z });
					police->SetRotation(80.0f, 0.0f, 0.0f);
					police->SetScale(Vec3(0.5f, 0.5f, 0.5f));
					police->SetRotation(80.0f, 0.0f, 0.0f);
					++count;
				}
				

				z += 100.0f;
			}

			if (z > 4000.0f)
			{
				x -= 100.0f;
				z = 1000.0f;
				if (x < -3300.0f)
				{
					x = -300.0f;
					y += 100.0f;
				}
			}

			time -= 0.016f;
		}
	}

	std::vector<SPtr<VisualOctree>> PlayerTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> PlayerTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}
