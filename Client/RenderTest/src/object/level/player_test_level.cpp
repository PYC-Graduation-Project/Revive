#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/mesh_octree.h>
#include "object/level/player_test_level.h"

namespace render_test
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


		auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

	/*	police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 300.0f, 0.0f, 1000.0f });

		police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ -300.0f, 0.0f, 1000.0f });*/

		police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ -600.0f, 0.0f, 1000.0f });

		police->SetScale(0.2f);

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
		
		/*static float x = -300.0f, y = 0.0f, z = 1100.0f;
		static float time = 0.0f;

		static UINT count = 1;

		time += delta_time;

		if (time >= 0.016f && count <= 6000)
		{
			for (int i = 0; i < 10; ++i)
			{
				auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kDestructable, "../Contents/police.obj");
				SpawnActor(police);
				police->SetPosition(Vec3{ x, y, z });
				police->SetScale(0.2f);
				++count;

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
		}*/

		/*if (count % 100 == 1)
			LOG_INFO(count);*/
	}

	SPtr<MeshOctree> PlayerTestLevel::CreateMeshOctree() const
	{
		return CreateSPtr<MeshOctree>(10000.0f);
	}
}
