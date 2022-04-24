#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/core/actor.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/event/messageevent/message_helper.h>
#include <client/object/actor/sky_cube.h>
#include "object/level/game_play_level.h"
#include "object/gamemode/revive_game_mode.h"
#include"revive_server/message/message_event_info.h"
#include"server/network_move_object.h"


#include "object/actor/character/skeleton_King.h"
#include "object/actor/character/skeleton_soldier.h"
#include "object/actor/character/revive_player.h"


namespace revive
{

	GamePlayLevel::GamePlayLevel()
		: Level("game play level")
	{
	}
	bool GamePlayLevel::Initialize()
	{

		m_actors = m_map_loader.LoadMap("Contents/map.txt",eMapLoadType::kClient);
		for (auto& actor : m_actors)
		{
			SpawnActor(actor);
		}
		
		auto sky_cube = CreateSPtr<SkyCube>("Contents/grasscube1024.dds");
		SpawnActor(sky_cube);

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
		
	}
	void GamePlayLevel::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{

		switch (message->GetEventID())
		{
		case HashCode("testspawn"):
		{
			//auto cube = CreateSPtr<RotatingCube>();
			//SpawnActor(cube);
			auto msg = std::static_pointer_cast<TestMessageEventInfo>(message);
		    //cube->SetPosition(msg->GetPosition());
			//PacketHelper::ConnectActorToServer(cube, msg->GetObjId());
			break;
		}
		case HashCode("spawn object"):
		{

			auto msg = std::static_pointer_cast<ObjectInfoMessageEventInfo>(message);

			auto obj = msg->GetNetworkObj();
			LOG_INFO("id : {0}", obj->GetID());
			LOG_INFO("name : {0}", obj->GetName());
			std::cout << "pos: " << obj->GetPosition() << std::endl;
			switch ((NW_OBJ_TYPE)obj->GetType())
			{
			case NW_OBJ_TYPE::OT_BASE: {
				break;
			}
			case NW_OBJ_TYPE::OT_PLAYER: {
				auto player = CreateSPtr<DefaultPlayer>("other player");
				SpawnActor(player);
				player->SetPosition(obj->GetPosition());
				player->SetNetworkID(obj->GetID());
				PacketHelper::ConnectActorToServer(player, msg->GetNetworkObj()->GetID());
				break;
			}
			
			case NW_OBJ_TYPE::OT_NPC_SKULL: {
				auto police = CreateSPtr<SkeletonSoldier>();
				SpawnActor(police);
				police->SetPosition(obj->GetPosition());
				police->SetNetworkID(obj->GetID());
				PacketHelper::ConnectActorToServer(police, msg->GetNetworkObj()->GetID());

				break;
			}
			case NW_OBJ_TYPE::OT_NPC_SKULLKING: {
				auto police = CreateSPtr<SkeletonKing>();
				SpawnActor(police);
				police->SetPosition(obj->GetPosition());
				police->SetNetworkID(obj->GetID());
				PacketHelper::ConnectActorToServer(police, msg->GetNetworkObj()->GetID());
				break;
			}
			}

			break;
		}

		default:
			break;
		}
	}
	UPtr<GameMode> GamePlayLevel::CreateGameMode() const
	{
		return CreateUPtr<ReviveGameMode>();
	}
	std::vector<SPtr<VisualOctree>> GamePlayLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		//박스의 HalfWidth, 중심좌표
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(5000.0f,Vec3(2500.0f,0,2500.0f))); //Castle
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(5000.0f,Vec3(2500,0,7500.0f))); //Bridge + Spawn Area
		return visual_octrees;
	}
	std::vector<SPtr<CollisionOctree>> GamePlayLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(4800.0f, Vec3(2400.0f, 0, 2400.0f),0));
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(3600.0f, Vec3(2400.0f, 0, 6600.0f),0));
		//collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(5000.0f, Vec3(2500.0f, 0, 12500.0f),0)); //가로는 2500만큼 감싸고(양옆) 세로는 1000만큼 감싸야하지만 정사각형이므로 2500만큼 감싼다.
		return collision_octrees;
	}
}