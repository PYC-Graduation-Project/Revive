#include "stdafx.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/actor/core/actor.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	CameraComponent::CameraComponent(const std::string& name, eCameraUsage usage)
		: Component(name), m_camera_state(eCameraState::kPaused)
		, m_camera_usage(usage), m_projection_mode(eProjectionMode::kPerspective)
	{
	}

	bool CameraComponent::Initialize()
	{
		return RegisterToRenderSystem();
	}

	void CameraComponent::Shutdown()
	{
		UnregisterFromRenderSystem();
	}

	void CameraComponent::UpdateWorldMatrix()
	{
		const auto& owner = GetOwner().lock();
		Vec3 eye = owner->GetPosition();
		Vec3 target, up;
		if (m_owner_controller.expired())
		{
			target = eye + owner->GetForward();
			up = owner->GetUp();
		}
		else
		{
			target = eye + m_owner_controller.lock()->GetForward();
			up = m_owner_controller.lock()->GetUp();

		}

		m_view_matrix = mat4::LookAt(eye, target, up);
		m_inverse_view_matrix = mat4::Inverse(m_view_matrix);
		m_bf_projection.Transform(m_bounding_frustum, XMLoadFloat4x4(&m_inverse_view_matrix));
	}

	void CameraComponent::UpdateProjectionMatrix()
	{
		switch (m_projection_mode)
		{
		case client_fw::eProjectionMode::kPerspective:
			m_projection_matrix = mat4::Perspective(math::ToRadian(m_field_of_view), m_aspect_ratio, m_near_z, m_far_z);
			BoundingFrustum::CreateFromMatrix(m_bf_projection, XMLoadFloat4x4(&m_projection_matrix));
			break;
		case client_fw::eProjectionMode::kOrthographic:
			//m_projection_matrix = mat4::Ortho()
			break;
		}
	}

	bool CameraComponent::RegisterToRenderSystem()
	{
		if (Render::RegisterCameraComponent(shared_from_this()))
		{
			UpdateProjectionMatrix();
			return true;
		}
		return false;
	}

	void CameraComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterCameraComponent(shared_from_this());
	}

}
