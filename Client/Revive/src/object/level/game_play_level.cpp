#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/core/actor.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include "object/level/game_play_level.h"
//#include "object/actor/ground.h"

namespace revive
{
	GamePlayLevel::GamePlayLevel()
		: Level("game play level")
	{
	}
	bool GamePlayLevel::Initialize()
	{
		m_player = CreateSPtr<DefaultPawn>();
		m_player_controller = CreateSPtr<PlayerController>();

		m_player_controller->Possess(m_player);

		SpawnActor(m_player);
		SpawnActor(m_player_controller);

		auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });
		m_actors = m_map_loader.LoadMap("../Contents/Revive/map.txt",eMapLoadType::kClient);
		for (auto& actor : m_actors)
		{
			SpawnActor(actor);
			actor->SetPosition(Vec3{ 0.0f,0.0f,1000.0f });
		}
		

		Input::SetInputMode(eInputMode::kUIAndGame);
		Input::SetHideCursor(true);

		return true;
	}
	void GamePlayLevel::Shutdown()
	{
		Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);
	}
	void GamePlayLevel::Update(float delta_time)
	{
	}
	std::vector<SPtr<VisualOctree>> GamePlayLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}
	std::vector<SPtr<CollisionOctree>> GamePlayLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}