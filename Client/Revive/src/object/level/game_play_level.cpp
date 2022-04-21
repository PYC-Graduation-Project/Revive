#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/core/actor.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include "object/level/game_play_level.h"
#include <client/object/level/gamemode/game_mode_base.h>
#include "object/gamemode/revive_game_mode.h"
#include <client/event/packetevent/packet_helper.h>
#include <client/event/messageevent/message_helper.h>
#include"revive_server/message/message_event_info.h"
#include"server/network_move_object.h"


#include "object/actor/character/skeleton_King.h"
#include "object/actor/character/skeleton_soldier.h"
#include "object/actor/character/revive_player.h"
std::string g_id;
std::string g_pw;


namespace revive
{

	GamePlayLevel::GamePlayLevel()
		: Level("game play level")
	{
	}
	bool GamePlayLevel::Initialize()
	{

		//auto police = CreateSPtr<SkeletonSoldier>();
		//SpawnActor(police);
		//police->SetPosition(Vec3{ 2400.0f , 300.0f, 1200.0f });
		for (int i = 0; i < 15; ++i)
		{
			auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "Contents/cliff_block_rock.obj");
			police->SetScale(300.0f); 
			police->SetPosition(Vec3{ -500.0f + i * 500.0f, 0.0f, 6000.0f  });
			SpawnActor(police);
		}
		

		m_actors = m_map_loader.LoadMap("Contents/map.txt",eMapLoadType::kClient);
		for (auto& actor : m_actors)
		{
			SpawnActor(actor);
		}
		std::cin >> g_id;
		std::cin >> g_pw;
		std::cout << "id:" << g_id << "pw:" << g_pw << std::endl;

		//회원 가입 5, 로그인 6, 매칭 7
		RegisterPressedEvent("send sign up", { { eKey::k5 } },
			[this]()->bool {

				PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignUpMessageEventInfo>(HashCode("send sign up"), g_id.data(), g_pw.data()));
				return true;
			});
		RegisterPressedEvent("send sign in", { { eKey::k6 } },
			[this]()->bool {

				PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignInMessageEventInfo>(HashCode("send sign in"), g_id.data(), g_pw.data()));
				return true;
			});
		RegisterPressedEvent("send sign matching", { { eKey::k7 } },
			[this]()->bool {

				PacketHelper::RegisterPacketEventToServer(CreateSPtr<MatchingMessageEventInfo>(HashCode("send sign matching"), 2));
				return true;
			});

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
				break;
			}
			case NW_OBJ_TYPE::OT_NPC_SKULL: {
				auto police = CreateSPtr<SkeletonSoldier>();
				SpawnActor(police);
				police->SetPosition(obj->GetPosition());
				PacketHelper::ConnectActorToServer(police, msg->GetNetworkObj()->GetID());

				break;
			}
			case NW_OBJ_TYPE::OT_NPC_SKULLKING: {
				auto police = CreateSPtr<SkeletonKing>();
				SpawnActor(police);
				police->SetPosition(obj->GetPosition());
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