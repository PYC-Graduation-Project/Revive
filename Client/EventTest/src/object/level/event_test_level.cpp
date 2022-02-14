#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
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
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 330.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 360.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 390.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 300.0f, 30.0f, 1000.0f });
		sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/monkey.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ 500.0f, 60.0f, 2000.0f });
		sphere->SetScale(10.0f);

		sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/penguin.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ 1500.0f, 0.0f, 1500.0f });
		sphere->SetScale(200.0f);

		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ -500.0f, 0.0f, 800.0f });

		auto move_cube = CreateSPtr<MoveCube>();
		SpawnActor(move_cube);
		move_cube->SetPosition(Vec3{ 0.0f, 0.0f, 500.0f });
		m_spawn_pos = Vec3(0.0f, 0.0f, 600.0f);

		auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 0.0f, -110.0f, 650.0f });

		Input::SetInputMode(eInputMode::kUIAndGame);
		Input::SetHideCursor(true);

		RegisterPressedEvent("spawn movable actor", { EventKeyInfo{eKey::kP} },
			[this]()->bool {
				auto move_cube = CreateSPtr<MoveCube>();
				SpawnActor(move_cube);
				move_cube->SetPosition(m_spawn_pos);
				m_spawn_pos += Vec3(0.0f, 0.0f, 100.0f);
				return true;
			});


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

	std::vector<SPtr<VisualOctree>> EventTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> EventTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}