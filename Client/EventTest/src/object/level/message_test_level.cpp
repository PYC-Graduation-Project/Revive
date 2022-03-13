#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/util/octree/octree.h>
#include <client/event/messageevent/message_helper.h>
#include "object/level/message_test_level.h"
#include "message/message_event_info.h"
#include "object/actor/rotating_cube.h"

#include <client/event/packetevent/packet_helper.h>

namespace event_test
{
	MessageTestLevel::MessageTestLevel()
		: Level("message test level")
	{
	}

	bool MessageTestLevel::Initialize()
	{
		auto player = CreateSPtr<DefaultPawn>();
		auto controller = CreateSPtr<PlayerController>();

		controller->Possess(player);
		SpawnActor(player);
		SpawnActor(controller);

		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ -500.0f, 0.0f, 800.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 500.0f, 0.0f, 800.0f });

		RegisterAxisEvent("rotating speed up", { {eKey::kLArrow, -1.0f}, {eKey::kRArrow, 1.0f} },
			[this](float axis)->bool {
				MessageHelper::RegisterMessageEvent(CreateSPtr<RotSpeedMessageEventInfo>(HashCode("change rotating speed"), axis));
				return true;
			});

		//임시 코드 서버 참고용
		RegisterPressedEvent("spawn rotating cube", { { eKey::kP } },
			[this]()->bool {
				PacketHelper::RegisterPacketEventToLevel(CreateSPtr<MessageEventInfo>(HashCode("spawn rotating cube")));
				return true;
			});

		RegisterPressedEvent("remove rotating cube", { { eKey::kO } },
			[this]()->bool {
				PacketHelper::RegisterPacketEventToLevel(CreateSPtr<MessageEventInfo>(HashCode("remove rotating cube")));
				return true;
			});

		RegisterPressedEvent("change rotating speed", { {eKey::kC} }, [this]()->bool {
			PacketHelper::RegisterPacketEventToActor(CreateSPtr<MessageEventInfo>(HashCode("change rotating speed")), 0);
			return true; 
			});
		

		Input::SetInputMode(eInputMode::kGameOnly);
		return true;
	}

	void MessageTestLevel::Shutdown()
	{
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void MessageTestLevel::Update(float delta_time)
	{
	}

	void MessageTestLevel::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		static UINT id = 0;
		static Vec3 spawn_pos = Vec3(100.0f, 0.0f, 1100.0f);

		switch (message->GetEventID())
		{
		case HashCode("spawn rotating cube"):
		{
			auto cube = CreateSPtr<RotatingCube>();
			SpawnActor(cube);
			cube->SetPosition(spawn_pos);
			PacketHelper::ConnectActorToServer(cube, id++);
			spawn_pos += Vec3(100.0f, 0.0f, 100.0f);
			break;
		}
		case HashCode("remove rotating cube"):
		{
			if (id > 0)
			{
				const auto& actor = PacketHelper::DisconnectActorFromServer(--id);
				actor->SetActorState(eActorState::kDead);
				spawn_pos -= Vec3(100.0f, 0.0f, 100.0f);
			}
			break;
		}
		default:
			break;
		}
		
	}

	std::vector<SPtr<VisualOctree>> MessageTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> MessageTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}