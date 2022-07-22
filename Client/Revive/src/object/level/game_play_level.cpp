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
#include <client/asset/sound/core/sound_manager.h>

#include "object/level/game_play_level.h"
#include "object/level/sharedinfo/revive_level_shared_info.h"
#include "object/gamemode/revive_game_mode.h"

#include "object/actor/character/revive_controller.h"
#include "object/actor/character/skeleton_King.h"
#include "object/actor/character/skeleton_soldier.h"
#include "object/actor/character/revive_player.h"

#include "revive_server/message/message_event_info.h"
#include "server/network_move_object.h"

#include "object/actor/visual/torch.h"
#include "object/actor/visual/scope_light.h"
#include "object/actor/visual/light_tree.h"
#include "object/actor/healer.h"
#include "object/ui/game_end_ui_layer.h"
#include"server/network.h"
#include "object/ui/player_info_ui_layer.h"
#include "object/ui/game_info_ui_layer.h"

#include "object/effect/heal_effect.h"

std::string g_id;
std::string g_pw;

namespace revive
{
	GamePlayLevel::GamePlayLevel()
		: Level("game play level")
	{
		m_player_info_ui_layer = CreateSPtr<PlayerInfoUILayer>();
		m_game_info_ui_layer = CreateSPtr<GameInfoUILayer>();

	}

	bool GamePlayLevel::Initialize()
	{
		const auto& map_loader = std::static_pointer_cast<ReviveLevelSharedInfo>(LevelManager::GetLevelManager().GetLevelSharedInfo())->GetMapLoader();

		m_actors = map_loader->LoadMap("Contents/map.txt");
		for (auto& actor : m_actors)
		{
			SpawnActor(actor);
		}

		client_fw::SoundManager::GetSoundManager().Play(eSoundType::kBackGroundSound,"ingame sound");

		GenerateVisualActors();

		/*for (int i = 0; i < 1; ++i)
		{
			auto skel = CreateSPtr<SkeletonKing>();
			skel->SetNetworkPosition(Vec3(2300.0f, 300.0f, 3600.0f + i * 100.0f));
			SpawnActor(skel);
			auto soldier = CreateSPtr<SkeletonSoldier>();
			soldier->SetNetworkPosition(Vec3(2500.0f, 300.0f, 4000.0f + i * 100.0f));
			SpawnActor(soldier);
		}*/

		Input::SetInputMode(eInputMode::kGameOnly);

		RegisterPressedEvent("game menu", { { eKey::kEscape } },
			[this]()->bool {
				if (Input::GetInputMode() == eInputMode::kGameOnly)
				{
					m_game_end_ui_layer = CreateSPtr<GameEndUILayer>();
					m_game_end_ui_layer->SetGameState(eGameState::kRun);
					RegisterUILayer(m_game_end_ui_layer); 
					Input::SetInputMode(eInputMode::kUIAndGame);
					return true;
				}
				else if(Input::GetInputMode() == eInputMode::kUIAndGame)
				{
					m_game_end_ui_layer->SetUILayerState(eUILayerState::kDead);
					Input::SetInputMode(eInputMode::kGameOnly);
					return true;
				}
				return false;
			});
		
		/*RegisterPressedEvent("select music", { { eKey::kRArrow } },
			[this]()->bool {
				client_fw::SoundManager::GetSoundManager().Play(eSoundType::kBackGroundSound, m_bgm_list[m_index++]);
				if (m_index >= m_bgm_list.size())
					m_index = 0;
				return true;
			});*/
		RegisterUILayer(m_player_info_ui_layer);
		RegisterUILayer(m_game_info_ui_layer);
		PacketHelper::RegisterPacketEventToServer(CreateSPtr<GameStartEventInfo>(HashCode("game start")));

		return true;
	}

	void GamePlayLevel::GenerateVisualActors()
	{
		const auto& game_option = std::static_pointer_cast<ReviveLevelSharedInfo>(LevelManager::GetLevelManager().GetLevelSharedInfo())->GetGameOption();

		auto sky_cube = CreateSPtr<SkyCube>("Contents/night_sky_cube.dds");
		SpawnActor(sky_cube);

		auto d_light = CreateSPtr<DirectionalLight>();
		d_light->SetLightColor(Vec3(0.4f, 0.4f, 0.4f));
		d_light->SetRotation(math::ToRadian(20.0f), math::ToRadian(15.0f), 0.0f);
		SpawnActor(d_light);
		if(game_option->shadow_enable == false)
			d_light->DisableShadow();
		else
		{
			switch (game_option->shadow_quality)
			{
			case eShadowQuality::kLow: d_light->SetShadowTextureSize(2048); break;
			case eShadowQuality::kMiddle: d_light->SetShadowTextureSize(4096); break;
			case eShadowQuality::kHigh: d_light->SetShadowTextureSize(8192); break;
			case eShadowQuality::kVeryHigh: d_light->SetShadowTextureSize(8192); break;
			default: break;
			}
		}
		
		INT shadow_size = 64;

		switch (game_option->shadow_quality)
		{
		case eShadowQuality::kLow: shadow_size = 0; break;
		case eShadowQuality::kMiddle: shadow_size = 128; break;
		case eShadowQuality::kHigh: shadow_size = 256; break;
		case eShadowQuality::kVeryHigh:  shadow_size = 512; break;
		default: break;
		}

		auto CreateTorch = [this, game_option, shadow_size](Vec3&& position, Quaternion&& rotation = Quaternion())
		{
			auto torch = CreateSPtr<Torch>();
			SpawnActor(torch);
			torch->SetPosition(position);
			torch->SetRotation(rotation);
			if (game_option->shadow_enable == false)
				torch->DisableShadow();
			else
				torch->SetShadowTextureSize(shadow_size);
		};

		for (int i = 0; i < 11; ++i)
		{
			CreateTorch(Vec3(880.0f, 750.0f + (i % 2) * 50.0f, 900.0f + i * 300.0f), 
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(180.0f), 0.0f));
		}

		for (int i = 0; i < 11; ++i)
		{
			CreateTorch(Vec3(3920.0f, 750.0f + (i % 2) * 50.0f, 900.0f + i * 300.0f));
		}

		for (int i = 0; i < 9; ++i)
		{
			CreateTorch(Vec3(1200.0f + i * 300.0f, 750.0f + (i % 2) * 50.0f, 580.0f),
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(90.0f), 0.0f));
		}

		for (int i = 0; i < 2; ++i)
		{
			CreateTorch(Vec3(1200.0f + i * 2400.0f, 800.0f, 4525.0f),
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(-90.0f), 0.0f));
		}

		for (int i = 0; i < 4; ++i)
		{
			CreateTorch(Vec3(1600.0f + (i / 2) * 400.0f + (i % 2) * 1200.0f, 700.0f, 4225.0f),
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(-90.0f), 0.0f));
		}

		for (int i = 0; i < 6; ++i)
		{
			CreateTorch(Vec3(900.0f + (i % 3) * 550.0f + (i / 3) * 1900.0f, 700.0f, 4800.0f),
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(90.0f), 0.0f));
		}		

		switch (game_option->shadow_quality)
		{
		case eShadowQuality::kLow: shadow_size = 0; break;
		case eShadowQuality::kMiddle: shadow_size = 200; break;
		case eShadowQuality::kHigh: shadow_size = 400; break;
		case eShadowQuality::kVeryHigh:  shadow_size = 600; break;
		default: break;
		}

		auto CreateFenchTorch = [this, game_option, shadow_size](Vec3&& position, bool use_shadow, Quaternion&& rotation = Quaternion())
		{
			auto torch = CreateSPtr<FenceTorch>(use_shadow);
			SpawnActor(torch);
			torch->SetPosition(position);
			torch->SetRotation(rotation);
			if (game_option->shadow_enable == false)
				torch->DisableShadow();
			else
				torch->SetShadowTextureSize(shadow_size);
		};

		for (int i = 0; i < 36; ++i)
		{
			CreateFenchTorch(Vec3(2120.0f, 370.0f, 4870.0f + 290.0f * (i / 3) + 82.5f * (i % 3)), true);
		}

		for (int i = 0; i < 36; ++i)
		{
			CreateFenchTorch(Vec3(2680.0f, 370.0f, 4870.0f + 290.0f * (i / 3) + 82.5f * (i % 3)), true,
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(180.0f), 0.0f));
		}

		for (int i = 0; i < 10; ++i)
		{
			CreateFenchTorch(Vec3(1000.0f + i * 300.0f, 1025.0f, 4800.0f), false,
				quat::CreateQuaternionFromRollPitchYaw(0.0f, math::ToRadian(90.0f), 0.0f));
		}

		switch (game_option->shadow_quality)
		{
		case eShadowQuality::kLow: shadow_size = 256; break;
		case eShadowQuality::kMiddle: shadow_size = 512; break;
		case eShadowQuality::kHigh: shadow_size = 1024; break;
		case eShadowQuality::kVeryHigh:  shadow_size = 2048; break;
		default: break;
		}

		for (int i = 0; i < 4; ++i)
		{
			auto light_tree = CreateSPtr<LightTree>();
			SpawnActor(light_tree);
			light_tree->SetPosition(Vec3(2050.0f + (i / 2) * 700.0f, 300.0f, 1000.0f + (i % 2) * 700.0f));
			if (i < 2)
				light_tree->SetRotation(0.0f, math::ToRadian(135.0f + i * 90.0f), 0.0f);
			else
				light_tree->SetRotation(0.0f, math::ToRadian(135.0f - (i - 1) * 90.0f), 0.0f);
			if (game_option->shadow_enable == false)
				light_tree->DisableShadow();
			else
				light_tree->SetShadowTextureSize(shadow_size);
		}

		switch (game_option->shadow_quality)
		{
		case eShadowQuality::kLow: shadow_size = 400; break;
		case eShadowQuality::kMiddle: shadow_size = 800; break;
		case eShadowQuality::kHigh: shadow_size = 1600; break;
		case eShadowQuality::kVeryHigh:  shadow_size = 3200; break;
		default: break;
		}

		for (int i = 0; i < 2; ++i)
		{
			auto scope = CreateSPtr<ScopeLight>();
			SpawnActor(scope);
			scope->SetPosition(Vec3(1700.0f + i * 1400.0f, 1000.0f, 4300.0f));
			scope->SetRotation(0.0f, math::ToRadian(-30.0f + i * 60.0f), 0.0f);
			if (game_option->shadow_enable == false)
				scope->DisableShadow();
			else
				scope->SetShadowTextureSize(shadow_size);
		}

		m_heal_effect = CreateSPtr<HealEffect>();

		auto healer = CreateSPtr<Healer>();
		healer->SetPosition(Vec3(2400.0f, 300.0f, 1350.0f));
		healer->RegisterEffect(m_heal_effect);
		SpawnActor(healer);

	}

	void GamePlayLevel::Shutdown()
	{
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void GamePlayLevel::Update(float delta_time)
	{
		m_heal_effect->Update(delta_time);
	}

	void GamePlayLevel::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
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
				//LOG_INFO("나 소환");
				auto player = std::dynamic_pointer_cast<RevivePlayer>(GetGameMode()->GetDefaultPawn());
				player->SetPosition(obj->GetPosition());
				player->SetNetworkID(obj->GetID());
				player->SetPlayerName(obj->GetName());
				player->SetHP(obj->GetHp());
				player->SetMaxHP(obj->GetMaxHp());
				player->SetColor(obj->GetColorType());
				PacketHelper::ConnectActorToServer(player, msg->GetNetworkObj()->GetID());
				Network::matching_end = true;

				m_player_info_ui_layer->SetOwnerPlayer(player);
				break;
			}
			case NW_OBJ_TYPE::OT_PLAYER: {
				//LOG_INFO("느그 소환");
				auto player = CreateSPtr<DefaultPlayer>("other player");
				SpawnActor(player);
				player->SetNetworkPosition(obj->GetPosition());
				player->SetNetworkID(obj->GetID());
				player->SetPlayerName(obj->GetName());
				player->SetHP(obj->GetHp());
				player->SetMaxHP(obj->GetMaxHp());
				player->SetColor(obj->GetColorType());
				PacketHelper::ConnectActorToServer(player, msg->GetNetworkObj()->GetID());

				m_player_info_ui_layer->RegisterOtherPlayer(player);
				break;
			}

			case NW_OBJ_TYPE::OT_NPC_SKULL: {
				auto skull = CreateSPtr<SkeletonSoldier>();
				SpawnActor(skull);
				skull->SetNetworkPosition(obj->GetPosition());
				skull->SetNetworkID(obj->GetID());
				skull->SetHP(obj->GetHp());
				skull->SetMaxHP(obj->GetMaxHp());
				m_game_info_ui_layer->RegisterEnemy(skull);
				PacketHelper::ConnectActorToServer(skull, msg->GetNetworkObj()->GetID());
				break;
			}
			case NW_OBJ_TYPE::OT_NPC_SKULLKING: {
				auto king = CreateSPtr<SkeletonKing>();
				SpawnActor(king);
				king->SetNetworkPosition(obj->GetPosition());
				king->SetNetworkID(obj->GetID());
				king->SetHP(obj->GetHp());
				king->SetMaxHP(obj->GetMaxHp());
				m_game_info_ui_layer->RegisterEnemy(king);
				PacketHelper::ConnectActorToServer(king, msg->GetNetworkObj()->GetID());
				break;
			}
			}
			break;
		}
		case HashCode("win"):
		{
			m_game_end_ui_layer = CreateSPtr<GameEndUILayer>();
			m_game_end_ui_layer->SetGameState(eGameState::kWin);
			RegisterUILayer(m_game_end_ui_layer);
			Input::SetInputMode(eInputMode::kUIOnly);
			break;
		}
		case HashCode("defeat"):
		{
			m_game_end_ui_layer = CreateSPtr<GameEndUILayer>();
			m_game_end_ui_layer->SetGameState(eGameState::kDefeat);
			RegisterUILayer(m_game_end_ui_layer);
			Input::SetInputMode(eInputMode::kUIOnly);
			break;
		}
		case HashCode("wave info"):
		{
			auto msg = std::static_pointer_cast<WaveInfoMessageEventInfo>(message);
			
			m_game_info_ui_layer->SetWaveTime(msg->GetWaveTime());
			break;
		}
		case HashCode("next wave info"):
		{
			auto msg = std::static_pointer_cast<NextWaveInfoMessageEventInfo>(message);

			m_game_info_ui_layer->SetNextWaveCount(msg->GetSoldierNum(), msg->GetKingNum());
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
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(4800.0f, Vec3(2400.0f, 0.f, 2400.0f), 2)); //Castle
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(3600.0f, Vec3(2400.0f, 0.f, 6600.0f), 2)); //Bridge + Spawn Area
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f, Vec3(2400.0f, 0.f, 16800.0f), 0));
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(-13800.0f,0,12000.0f),0)); //Left Ground
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(18600.0f,0,12000.0f),0)); //Right Ground
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(-13800.0f,0,1200.0f),0)); //Left Ground2
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(16800.0f,Vec3(18600.0f,0,1200.0f),0)); //Right Ground2
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> GamePlayLevel::CreateCollisionOctrees() const
	{

		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(4800.0f, Vec3(2400.0f, 0.f, 2400.0f), 0));
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(3600.0f, Vec3(2400.0f, 0.f, 6600.0f), 0));
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(2400.0f, Vec3(2400.0f, 0.f, 9600.0f), 0));
		//collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(5000.0f, Vec3(2500.0f, 0, 12500.0f),0)); //가로는 2500만큼 감싸고(양옆) 세로는 1000만큼 감싸야하지만 정사각형이므로 2500만큼 감싼다.
		return collision_octrees;
	}

	/*void GamePlayLevel::LogInfoUI(const std::wstring& text)
	{
		m_debugging_ui_layer->LogInfoUI(text);
	}*/

}