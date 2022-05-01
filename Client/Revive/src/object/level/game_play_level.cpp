#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/core/actor.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>
#include <client/event/messageevent/message_helper.h>
#include <client/object/actor/sky_cube.h>
#include <client/object/actor/light.h>

#include "object/actor/character/revive_controller.h"
#include "object/level/game_play_level.h"
#include "object/level/game_end_level.h"
#include "object/actor/character/revive_controller.h"
#include "object/gamemode/revive_game_mode.h"
#include"revive_server/message/message_event_info.h"
#include"server/network_move_object.h"


#include "object/actor/character/skeleton_King.h"
#include "object/actor/character/skeleton_soldier.h"
#include "object/actor/character/revive_player.h"

#include "object/actor/visual/torch.h"
#include "object/actor/visual/scope_light.h"
#include "object/actor/visual/light_tree.h"
#include "object/actor/healer.h"


std::string g_id;
std::string g_pw;

namespace revive
{
	GamePlayLevel::GamePlayLevel()
		: Level("game play level")
	{
		LOG_INFO("게임플레이레벨 생성");
	}

	bool GamePlayLevel::Initialize()
	{
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

				LOG_INFO("매칭 대기중...");
				PacketHelper::RegisterPacketEventToServer(CreateSPtr<MatchingMessageEventInfo>(HashCode("send sign matching"), 3));
			return true;
		});
		RegisterPressedEvent("game start", { { eKey::k8 } },
			[this]()->bool {
			LOG_INFO("게임 시작!");
			PacketHelper::RegisterPacketEventToServer(CreateSPtr<GameStartEventInfo>(HashCode("game start")));
			return true;
		});
		LOG_INFO("게임플레이레벨 초기화");

		m_actors = m_map_loader.LoadMap("Contents/map.txt",eMapLoadType::kClient);
		for (auto& actor : m_actors)
		{
			SpawnActor(actor);
		}
		
		RegisterPressedEvent("Game End", { { eKey::k0 } },
			[this]()->bool {
			//플레이어 사망 및 기지 파괴될 시 아래코드 사용
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<GameEndLevel>(eGameResult::kDefeat), nullptr);
			return true;
		});

		GenerateVisualActors();

		for (int i = 0; i < 50; ++i)
		{
			auto skel = CreateSPtr<SkeletonKing>();
			skel->SetNetworkPosition(Vec3(2400.0f, 300.0f, 3600.0f + i * 100.0f));
			SpawnActor(skel);
		}

		Input::SetInputMode(eInputMode::kGameOnly);
		return true;
	}

	void GamePlayLevel::GenerateVisualActors()
	{
		auto sky_cube = CreateSPtr<SkyCube>("Contents/night_sky_cube.dds");
		SpawnActor(sky_cube);

		auto d_light = CreateSPtr<DirectionalLight>();
		d_light->SetLightColor(Vec3(0.4f, 0.4f, 0.4f));
		d_light->SetRotation(math::ToRadian(20.0f), math::ToRadian(15.0f), 0.0f);
		SpawnActor(d_light);
		d_light->SetShadowTextureSize(8192);
		//d_light->DisableShadow();

		for (int i = 0; i < 11; ++i)
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(180.0f), 0.0f);
			torch->SetPosition(Vec3(880.0f, 750.0f + (i % 2) * 50.0f, 900.0f + i * 300.0f));
		}

		for (int i = 0; i < 11; ++i)
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetPosition(Vec3(3920.0f, 750.0f + (i % 2) * 50.0f, 900.0f + i * 300.0f));
		}

		for (int i = 0; i < 9; ++i)
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(90.0f), 0.0f);
			torch->SetPosition(Vec3(1200.0f + i * 300.0f, 750.0f + (i % 2) * 50.0f, 580.0f));
		}

		for (int i = 0; i < 2; ++i)
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(-90.0f), 0.0f);
			torch->SetPosition(Vec3(1200.0f + i * 2400.0f, 800.0f, 4525.0f));
		}

		for (int i = 0; i < 4; ++i)
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(-90.0f), 0.0f);
			torch->SetPosition(Vec3(1600.0f + (i / 2) * 400.0f + (i % 2) * 1200.0f, 700.0f, 4225.0f));
		}

		for (int i = 0; i < 6; ++i)
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(90.0f), 0.0f);
			torch->SetPosition(Vec3(900.0f + (i % 3) * 550.0f + (i / 3) * 1900.0f, 700.0f, 4800.0f));
		}		

		for (int i = 0; i < 36; ++i)
		{
			auto torch = CreateSPtr<FenceTorch>();
			SpawnActor(torch);
			//torch->SetRotation(0.0f, math::ToRadian(90.0f), 0.0f);
			torch->SetPosition(Vec3(2120.0f, 370.0f, 4870.0f + 290.0f * (i / 3) + 82.5f * (i % 3)));
			//torch->SetPosition(Vec3(2120.0f, 370.0f, 4870.0f + 290.0f * (i / 2) + 165.0f * (i % 2)));
		}

		for (int i = 0; i < 36; ++i)
		{
			auto torch = CreateSPtr<FenceTorch>();
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(180.0f), 0.0f);
			torch->SetPosition(Vec3(2680.0f, 370.0f, 4870.0f + 290.0f * (i / 3) + 82.5f * (i % 3)));
			//torch->SetPosition(Vec3(2680.0f, 370.0f, 4870.0f + 290.0f * (i / 2) + 165.0f * (i % 2)));
		}

		for (int i = 0; i < 10; ++i)
		{
			auto torch = CreateSPtr<FenceTorch>(false);
			SpawnActor(torch);
			torch->SetRotation(0.0f, math::ToRadian(90.0f), 0.0f);
			torch->SetPosition(Vec3(1000.0f + i * 300.0f, 1025.0f, 4800.0f));
		}

		for (int i = 0; i < 4; ++i)
		{
			auto light_tree = CreateSPtr<LightTree>();
			if(i < 2)
				light_tree->SetRotation(0.0f, math::ToRadian(135.0f + i * 90.0f), 0.0f);
			else
				light_tree->SetRotation(0.0f, math::ToRadian(135.0f - (i - 1) * 90.0f), 0.0f);

			light_tree->SetPosition(Vec3(2050.0f + (i / 2) * 700.0f, 300.0f, 1000.0f + (i % 2) * 700.0f));
			SpawnActor(light_tree);
		}

		auto healer = CreateSPtr<Healer>();
		healer->SetPosition(Vec3(2400.0f, 300.0f, 1350.0f));
		SpawnActor(healer);
	}

	void GamePlayLevel::Shutdown()
	{
		LOG_INFO("게임 플레이레벨 셧다운");
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
			case NW_OBJ_TYPE::OT_MY_PLAYER: {
				LOG_INFO("나 소환");
				auto player = std::dynamic_pointer_cast<RevivePlayer>(GetGameMode()->GetDefaultPawn());
				player->SetPosition(obj->GetPosition());
				player->SetNetworkID(obj->GetID());
				player->SetHP(obj->GetHp());
				PacketHelper::ConnectActorToServer(player, msg->GetNetworkObj()->GetID());

				break;
			}
			case NW_OBJ_TYPE::OT_PLAYER: {
				LOG_INFO("느그 소환");

				auto player = CreateSPtr<DefaultPlayer>("other player");
				SpawnActor(player);
				player->SetNetworkPosition(obj->GetPosition());
				player->SetNetworkID(obj->GetID());
				player->SetHP(obj->GetHp());
				PacketHelper::ConnectActorToServer(player, msg->GetNetworkObj()->GetID());
				
				break;
			}

			case NW_OBJ_TYPE::OT_NPC_SKULL: {
				auto skull = CreateSPtr<SkeletonSoldier>();
				SpawnActor(skull);
				skull->SetNetworkPosition(obj->GetPosition());
				skull->SetNetworkID(obj->GetID());
				skull->SetHP(obj->GetHp());
				PacketHelper::ConnectActorToServer(skull, msg->GetNetworkObj()->GetID());
				break;
			}
			case NW_OBJ_TYPE::OT_NPC_SKULLKING: {
				auto king = CreateSPtr<SkeletonKing>();
				SpawnActor(king);
				king->SetNetworkPosition(obj->GetPosition());
				king->SetNetworkID(obj->GetID());
				king->SetHP(obj->GetHp());
				PacketHelper::ConnectActorToServer(king, msg->GetNetworkObj()->GetID());
				break;
			}
			}
			break;
		}
		case HashCode("match"):
		{
			auto msg = std::static_pointer_cast<MatchingMessageOKEventInfo>(message);
			break;
		}
		case HashCode("sign in"):
		{
			auto msg = std::static_pointer_cast<SignInMessageOkEventInfo>(message);
			m_is_succeed_login = true;
			break;
		}
		case HashCode("win"):
		{
			auto msg = std::static_pointer_cast<GameWinEventInfo>(message);
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<GameEndLevel>(eGameResult::kWin), nullptr);
			break;
		}
		case HashCode("defeat"):
		{
			auto msg = std::static_pointer_cast<GameDefeatEventInfo>(message);
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<GameEndLevel>(eGameResult::kDefeat), nullptr);
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
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(4800.0f, Vec3(2400.0f, 0, 2400.0f), 2)); //Castle
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(4000.0f, Vec3(2400.0f, 0, 6600.0f), 2)); //Bridge + Spawn Area
		//visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(-13800.0f,0,12000.0f))); //Left Ground
		//visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(18600.0f,0,12000.0f))); //Right Ground
		//visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(-13800.0f,0,1200.0f))); //Left Ground2
		//visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(18600.0f,0,1200.0f))); //Right Ground2
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