#include "stdafx.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/actor/core/actor.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	RenderCameraComponent::RenderCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kBasic, name)
	{
	}

	void RenderCameraComponent::UpdateViewMatrix()
	{
		Vec3 target, up;
		if (m_owner_controller.expired() || (m_owner_controller.expired() == false && m_use_controller_rotation == false))
		{
			m_camera_position = GetWorldPosition();
			target = m_camera_position + GetWorldForward();
			up = GetWorldUp();
		}
		else
		{
			const auto& owner = m_owner.lock();
			Mat4 world_matrix = mat4::CreateScale(owner->GetScale());
			world_matrix *= mat4::CreateRotationFromQuaternion(m_owner_controller.lock()->GetRotation());
			world_matrix *= mat4::CreateTranslation(owner->GetPosition());

			m_camera_position = vec3::TransformCoord(m_local_position, world_matrix);
			target = m_camera_position + m_owner_controller.lock()->GetForward();
			up = m_owner_controller.lock()->GetUp();
		}

		m_view_matrix = mat4::LookAt(m_camera_position, target, up);
	}

	void RenderCameraComponent::SetMainCamera()
	{
		Render::SetMainCamera(SharedFromThis());
	}

	void RenderCameraComponent::SetOwnerController(const WPtr<Actor>& owner)
	{
		m_owner_controller = owner;
		UpdateWorldMatrix();
	}

	SPtr<RenderCameraComponent> RenderCameraComponent::SharedFromThis()
	{
		return std::static_pointer_cast<RenderCameraComponent>(shared_from_this());
	}
}
