#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/simple_movement_component.h>
#include <client/object/component/util/character_movement_component.h>
#include <client/event/messageevent/message_helper.h>

#include "message/message_event_info.h"
#include "interpolation_tester.h"

namespace event_test
{
	InterpolationTester::InterpolationTester()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/penguin.obj")
	{
		m_simple_movement_component = CreateSPtr<SimpleMovementComponent>();
		m_character_movement_component = CreateSPtr<CharacterMovementComponent>();
	}

	bool InterpolationTester::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_static_mesh_component->SetLocalScale(10.0f);
		m_static_mesh_component->SetLocalRotation(quat::CreateQuaternionFromAxis(vec3::AXIS_Y, math::ToRadian(180.0f)));
		m_simple_movement_component->SetMaxSpeed(145.0f);
		ret &= AttachComponent(m_simple_movement_component);
		m_character_movement_component->SetMaxSpeed(145.0f);
		m_character_movement_component->UseOrientRotationToMovement(true);
		ret &= AttachComponent(m_character_movement_component);
		return ret;
	}

	void InterpolationTester::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void InterpolationTester::Update(float delta_time)
	{
		m_time += delta_time;
		//m_simple_movement_component->AddInputVector(m_velocity);
		m_character_movement_component->AddInputVector(m_velocity);

		if (m_time >= 0.5f)
		{
			m_time -= 0.5f;

			m_inter_velocity = m_previous_pos - GetPosition();

			LOG_INFO(vec3::Length(m_previous_pos - GetPosition()));
		}

		SetPosition(GetPosition() + m_inter_velocity * delta_time * 2.0f);
	}

	void InterpolationTester::ExecuteMessage(const SPtr<MessageEventInfo>& message)
	{
	}

	void InterpolationTester::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("move object"): {
			auto msg = std::static_pointer_cast<MoveObjectMessageEventInfo>(message);

			m_previous_pos = m_next_pos;
			m_velocity = msg->GetObjPosition() - m_previous_pos;
			m_next_pos = msg->GetObjPosition();

			break;
		}
		default:
			break;
		}
	}
}