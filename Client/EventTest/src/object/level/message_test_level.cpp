#include <include/client_core.h>
#include <client/input/input.h>
#include <client/util/octree/octree.h>
#include <client/event/messageevent/message_helper.h>
#include "object/level/message_test_level.h"
#include "message/message_event_info.h"
#include "object/actor/rotating_cube.h"
#include"server/network_move_object.h"
#include <client/event/packetevent/packet_helper.h>
#include<object/actor/move_cube.h>
#include<mutex>
std::string g_id;
std::string g_pw;

namespace event_test
{
	MessageTestLevel::MessageTestLevel()
		: Level("message test level")
	{
	}

	bool MessageTestLevel::Initialize()
	{
		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ -500.0f, 0.0f, 800.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 500.0f, 0.0f, 800.0f });

		//auto m_cube = CreateSPtr<MoveCube>();
		//SpawnActor(m_cube);
		//m_cube->SetPosition(Vec3{ 0.0f,0.0f,0.0f });
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
		
		//아이디 입력 테스트	
		//std::cin >> g_id;
		//std::cin >> g_pw;
		//std::cout << "id:"<<g_id <<"pw:"<< g_pw << std::endl;
		
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
		RegisterPressedEvent("remove rotating cube", { { eKey::kO } },
			[this]()->bool {
				PacketHelper::RegisterPacketEventToLevel(CreateSPtr<MessageEventInfo>(HashCode("remove rotating cube")));
				return true;
			});

		RegisterPressedEvent("change rotating speed", { {eKey::kC} }, [this]()->bool {
			PacketHelper::RegisterPacketEventToActor(CreateSPtr<MessageEventInfo>(HashCode("change rotating speed")), 0);
			return true; 
			});

		return true;
	}

	void MessageTestLevel::Shutdown()
	{
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
		case HashCode("testspawn"):
		{
			auto cube = CreateSPtr<RotatingCube>();
			SpawnActor(cube);
			auto msg = std::static_pointer_cast<event_test::TestMessageEventInfo>(message);
			cube->SetPosition(msg->GetPosition());
			PacketHelper::ConnectActorToServer(cube, msg->GetObjId());
			spawn_pos += Vec3(100.0f, 0.0f, 100.0f);
			break;
		}
		case HashCode("spawn object"):
		{
			
			auto msg = std::static_pointer_cast<event_test::ObjectInfoMessageEventInfo>(message);
		
			auto obj =msg->GetNetworkObj();
			LOG_INFO("id : {0}",obj->GetID());
			LOG_INFO("name : {0}", obj->GetName());
			//LOG_INFO("hp : {0}", obj->GetHp());
			//LOG_INFO("damage : {0}", obj->GetDamage());
			//LOG_INFO("position :{0}", obj->GetPosition());
			std::cout <<"pos: " << obj->GetPosition() << std::endl;
			auto cube = CreateSPtr<RotatingCube>();
			SpawnActor(cube);
			LOG_INFO("rotation : {0}", cube->GetRotation());
			cube->SetPosition(msg->GetNetworkObj()->GetPosition());
			PacketHelper::ConnectActorToServer(cube, msg->GetNetworkObj()->GetID());
			
			//spawn_pos += Vec3(100.0f, 0.0f, 100.0f);
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