#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/event/messageevent/message_helper.h>
#include <client/util/octree/octree.h>
#include <client/asset/sound/core/sound_manager.h>
#include <client/object/actor/sky_cube.h>
#include <client/object/actor/light.h>
#include "client/object/component/util/render_camera_component.h"

#include "revive_server/message/message_event_info.h"
#include "object/level/game_play_level.h"
#include "object/level/lobby_level.h"
#include "object/ui/lobby_ui_layer.h"

#include "object/actor/visual/torch.h"
#include "object/actor/visual/scope_light.h"
#include "object/actor/visual/light_tree.h"
#include "object/actor/healer.h"
#include "object/actor/character/camera_actor.h"

#include "server/network_move_object.h"
#include"server/network.h"
//std::string g_id;
//std::string g_pw;

namespace revive
{
	LobbyLevel::LobbyLevel()
		:Level("lobby level")
	{
		 
	}
	bool LobbyLevel::Initialize()
	{
		client_fw::SoundManager::GetSoundManager().Play(eSoundType::kBackGroundSound,"lobby sound");
		m_lobby_ui_layer = CreateSPtr<LobbyUILayer>();
		RegisterUILayer(m_lobby_ui_layer);

		const auto& map_loader = std::static_pointer_cast<ReviveLevelSharedInfo>(LevelManager::GetLevelManager().GetLevelSharedInfo())->GetMapLoader();

		for (auto& actor : map_loader->LoadMap("Contents/map.txt"))
		{
			SpawnActor(actor);
		}
		GenerateVisualActors();

		auto lobby_camera = CreateSPtr<CameraActor>();

		lobby_camera->SetPosition(Vec3{2400.f,400.f,6000.f});
		lobby_camera->SetRotation(math::ToRadian(-10.f), math::ToRadian(180.f), 0.f);
		lobby_camera->SetMainCamera();
		SpawnActor(lobby_camera);
		Input::SetInputMode(eInputMode::kUIAndGame);
		Network::matching_end = false;
//#ifdef _DEBUG
		RegisterPressedEvent("develop mode", { {eKey::kJ, {	eAdditionalKey::kControl, eAdditionalKey::kShift }} },
			[this]()->bool { m_lobby_ui_layer->EnableDevelopMode(); return true; });
//#endif
		
		/*RegisterPressedEvent("music stop", { {eKey::kS} },
			[this]()->bool {client_fw::SoundManager::GetSoundManager().Stop(); return true; });
		
		RegisterPressedEvent("music pause", { {eKey::kP} },
			[this]()->bool {client_fw::SoundManager::GetSoundManager().Pause(); return true; });*/

		return true;
	}

	void LobbyLevel::Shutdown()
	{

	}

	void LobbyLevel::Update(float delta_time)
	{
	}
	void LobbyLevel::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("match"):
		{
			//auto msg = std::static_pointer_cast<MatchingMessageOKEventInfo>(message);
			m_lobby_ui_layer->SucceededMatching();
			break;
		}
		case HashCode("sign in"):
		{
			//auto msg = std::static_pointer_cast<SignInMessageOkEventInfo>(message);
			m_lobby_ui_layer->SucceededLogin();
			break;
		}
		case HashCode("sign up"):
		{
			m_lobby_ui_layer->SucceededSignUp();
			break;
		}
		case HashCode("login fail"):
		{
			auto msg = std::static_pointer_cast<LoginFailMessageEventInfo>(message);
			auto reason = msg->GetLoginFailType();

			switch (reason)
			{
			case eLoginFailType::kAlreadyLogin:
				m_lobby_ui_layer->FailedLogin(eLoginFailState::kAlreadyLogin);
				break;
			case eLoginFailType::kInvalidPW:
			case eLoginFailType::kInvalidID:
				m_lobby_ui_layer->FailedLogin(eLoginFailState::kInvalidIDPW);
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	void LobbyLevel::GenerateVisualActors()
	{
		const auto& game_option = std::static_pointer_cast<ReviveLevelSharedInfo>(LevelManager::GetLevelManager().GetLevelSharedInfo())->GetGameOption();

		auto sky_cube = CreateSPtr<SkyCube>("Contents/night_sky_cube.dds");
		SpawnActor(sky_cube);

		auto d_light = CreateSPtr<DirectionalLight>();
		d_light->SetLightColor(Vec3(0.4f, 0.4f, 0.4f));
		d_light->SetRotation(math::ToRadian(20.0f), math::ToRadian(15.0f), 0.0f);
		SpawnActor(d_light);
		if (game_option->shadow_enable == false)
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


		auto healer = CreateSPtr<Healer>();
		healer->SetPosition(Vec3(2400.0f, 300.0f, 1350.0f));
		SpawnActor(healer);
	}

	std::vector<SPtr<VisualOctree>> LobbyLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		//박스의 HalfWidth, 중심좌표
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(4800.0f, Vec3(2400.0f, 0.f, 2400.0f), 2)); //Castle
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(3600.0f, Vec3(2400.0f, 0.f, 4000.0f), 2)); //Bridge + Spawn Area
		return visual_octrees;
	}

}