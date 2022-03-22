#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/core/actor.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include "object/level/game_play_level.h"
#include <client/object/level/gamemode/game_mode_base.h>
#include "object/level/revive_game_mode.h"

namespace revive
{
	GamePlayLevel::GamePlayLevel()
		: Level("game play level")
	{
	}
	bool GamePlayLevel::Initialize()
	{
		//auto police = CreateSPtr<StaticMeshActor>( "../Contents/Cube.obj");
		//auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic,"Contents/fence_planksDouble.obj");
		//auto police = CreateSPtr<Ground>("Contents/cliff_block_rock.obj");
		////auto police = CreateSPtr<Ground>();
		//SpawnActor(police);
		m_actors = m_map_loader.LoadMap("Contents/map.txt",eMapLoadType::kClient);
		for (auto& actor : m_actors)
		{
			SpawnActor(actor);
		}
		

		Input::SetInputMode(eInputMode::kGameOnly);
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
		//LOG_INFO("player {0}", m_player->GetPosition());
	/*	for (auto& actor : m_actors)
		{
			LOG_INFO("{0} : {1}",actor->GetName(), actor->GetWorldMatrix());
		}*/
		
	}
	UPtr<GameMode> GamePlayLevel::CreateGameMode() const
	{
		return CreateUPtr<ReviveGameMode>();
	}
	std::vector<SPtr<VisualOctree>> GamePlayLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(20000.0f));
		return visual_octrees;
	}
	std::vector<SPtr<CollisionOctree>> GamePlayLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(30000.0f));
		return collision_octrees;
	}
}