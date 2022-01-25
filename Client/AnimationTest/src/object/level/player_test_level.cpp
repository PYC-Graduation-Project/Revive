#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include "object/level/player_test_level.h"

namespace animation_test
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


		auto gunner = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
		SpawnActor(gunner);
		gunner->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

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
	}
}
