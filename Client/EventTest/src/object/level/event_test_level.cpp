#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/mesh_octree.h>
#include "object/level/event_test_level.h"
#include "object/actor/rotating_cube.h"
#include "object/actor/move_cube.h"

namespace event_test
{
	EventTestLevel::EventTestLevel()
		: Level("event test level")
	{
	}

	bool EventTestLevel::Initialize()
	{
		auto player = CreateSPtr<DefaultPawn>();
		auto controller = CreateSPtr<PlayerController>();

		controller->Possess(player);
		SpawnActor(player);
		SpawnActor(controller);

		auto sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ 300.0f, 0.0f, 1000.0f });
		sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ 500.0f, 0.0f, 800.0f });

		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ -500.0f, 0.0f, 800.0f });

		auto move_cube = CreateSPtr<MoveCube>();
		SpawnActor(move_cube);
		move_cube->SetPosition(Vec3{ 0.0f, 0.0f, 500.0f });

		Input::SetInputMode(eInputMode::kUIAndGame);
		Input::SetHideCursor(true);

		return true;
	}

	void EventTestLevel::Shutdown()
	{
		Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void EventTestLevel::Update(float delta_time)
	{
	}

	SPtr<MeshOctree> EventTestLevel::CreateMeshOctree() const
	{
		return CreateSPtr<MeshOctree>(10000.0f);
	}
}