#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/rotating_movement_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/event/messageevent/message_helper.h>
#include "rotating_cube.h"
#include "message/message_event_info.h"

namespace event_test
{
	RotatingCube::RotatingCube()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/cube.obj")
	{
		m_rotating_component = CreateSPtr<RotatingMovementComponent>();
	}

	bool RotatingCube::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		ret &= AttachComponent(m_rotating_component);
		ret &= AttachComponent(CreateSPtr<SphereComponent>(200.0f));

		RegisterReceiveMessage(HashCode("change rotating speed"));

		return ret;
	}

	void RotatingCube::Shutdown()
	{
		m_rotating_component = nullptr;
		StaticMeshActor::Shutdown();
	}

	void RotatingCube::Update(float delta_time)
	{
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
		case HashCode("change rotating speed"):
			m_rotating_component->SetRotatingRate(Vec3(0.0f, 180.0f, 0.0f));
			//SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			break;
		default:
			break;
		}
	}
}