#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/rotating_movement_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/widget_component.h>
#include <client/object/component/util/camera_component.h>
#include <client/event/messageevent/message_helper.h>
#include "rotating_cube.h"
#include "object/ui/enemy_info_ui_layer.h"
#include "message/message_event_info.h"
using namespace std;
namespace event_test
{
	RotatingCube::RotatingCube()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/bricks_cube.obj")
	{
		m_ui_layer = CreateSPtr<EnemyInfoUILayer>();
		m_rotating_component = CreateSPtr<RotatingMovementComponent>();
		m_widget_component = CreateSPtr<WidgetComponent>(m_ui_layer);
	}

	bool RotatingCube::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_ui_layer->SetRotatingCube(std::static_pointer_cast<RotatingCube>(shared_from_this()));
		ret &= AttachComponent(m_rotating_component);
		ret &= AttachComponent(m_widget_component);
		m_widget_component->SetLocalPosition(Vec3(0.0f, 90.0f, 0.0f));
		m_widget_component->SetSize(Vec2(200.0f, 60.0f));
		//ret &= AttachComponent(CreateSPtr<CameraComponent>());3
		//m_rotating_component->SetRotatingRate(Vec3(0.0f, 0.0f, 0.0f));
		RegisterReceiveMessage(HashCode("change rotating speed"));
		RegisterReceiveMessage(HashCode("move object"));
		RegisterReceiveMessage(HashCode("testmove"));
		
		return ret;
	}

	void RotatingCube::Shutdown()
	{
		m_rotating_component = nullptr;
		m_widget_component = nullptr;
		m_ui_layer = nullptr;
		StaticMeshActor::Shutdown();
	}

	void RotatingCube::Update(float delta_time)
	{
		if (m_rotating_y_speed != m_rotating_component->GetRotatingRate().y)
		{
			m_rotating_y_speed = m_rotating_component->GetRotatingRate().y;
			if (m_speed_change_function != nullptr)
				m_speed_change_function(m_rotating_y_speed);
		}
		
		//--------방햑벡터 보내서 확인하는 작업
		//if (m_network_vec != Vec3(0.0f, 0.0f, 0.0f))
		//{
		//	Vec3 new_pos = GetPosition() + m_network_vec*m_nw_speed*delta_time;
		//	LOG_INFO(new_pos);
		//	SetPosition(new_pos);
		//}
		
	}

	void RotatingCube::ExecuteMessage(const SPtr<MessageEventInfo>& message)
	{
		if (message->GetEventID() == HashCode("change rotating speed"))
		{
			auto msg = std::static_pointer_cast<RotSpeedMessageEventInfo>(message);
			if (msg != nullptr)
			{
				Vec3 speed = m_rotating_component->GetRotatingRate();
				speed.y += msg->GetSpeed();
				m_rotating_component->SetRotatingRate(speed);
			}
		}

	}

	void RotatingCube::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("change rotating speed"): {
			m_rotating_component->SetRotatingRate(Vec3(0.0f, 180.0f, 0.0f));
			//SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			break;
		}
		case HashCode("move object"): {
			auto msg = std::static_pointer_cast<MoveObjectMessageEventInfo>(message);
			//msg->m_move_lock.lock();
			//m_rotating_component->SetRotatingRate(Vec3(0.0f, 180.0f, 0.0f));
			//SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			//SetPosition(msg->GetObjPosition());


			//진짜 다음좌표 보내는것
			SetPosition(msg->GetObjPosition());
			//std::cout << msg->GetObjPosition() << std::endl;
			//Vec3 a{ msg->GetObjPosition() };
			//LOG_INFO(a);
			//msg->m_move_lock.unlock();
			break;
		}
		case HashCode("testmove"): {
			auto msg = std::static_pointer_cast<TestMessageEventInfo>(message);
			//msg->m_move_lock.lock();
			//m_rotating_component->SetRotatingRate(Vec3(0.0f, 180.0f, 0.0f));
			//SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			//SetPosition(msg->GetObjPosition());
			
			//-----------방향벡터 보내는 테스트----------//
			//m_network_vec = msg->GetPosition();
			//------------------------------------------//
			 
		
			//std::cout << msg->GetObjPosition() << std::endl;
			//Vec3 a{ msg->GetObjPosition() };
			//LOG_INFO(a);
			//msg->m_move_lock.unlock();
			break;
		}
		default:
			break;
		}
	}
}