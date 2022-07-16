#include <include/client_core.h>
#include "client/object/component/util/render_camera_component.h"

#include "object/actor/character/camera_actor.h"

namespace revive
{
	CameraActor::CameraActor()
		:Actor(eMobilityState::kStatic,"Camera Actor")
	{
		m_camera_component = CreateSPtr<RenderCameraComponent>();
	}
	bool CameraActor::Initialize()
	{
		bool ret = true;

		ret &= AttachComponent(m_camera_component);
		
		return ret;
	}

	void CameraActor::Shutdown()
	{
	}
	void CameraActor::Update(float delta_time)
	{
	}
	void CameraActor::SetMainCamera()
	{
		m_camera_component->SetMainCamera();
	}
}