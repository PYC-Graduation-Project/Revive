#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "move_cube.h"

namespace event_test
{
	MoveCube::MoveCube()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/cube.obj")
	{
	}

	bool MoveCube::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_first_pos = GetPosition();
		return ret;
	}

	void MoveCube::Shutdown()
	{
	}

	void MoveCube::Update(float delta_time)
	{

		if (m_first_pos.x + 5000.0f < GetPosition().x)
		{
			m_speed = -2000.0f;
		}
		else if (m_first_pos.x - 5000.0f > GetPosition().x)
		{
			m_speed = 2000.0f;
		}

		Vec3 new_pos = GetPosition() + Vec3(delta_time * m_speed, 0.0f, 0.0f);
		SetPosition(new_pos);
	}
}