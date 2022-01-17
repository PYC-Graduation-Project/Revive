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
		Vec3 target = eye + owner->GetForward();
		Vec3 up = owner->GetUp();

		m_view_matrix = mat4::LookAt(eye, target, up);
	}

	void CameraComponent::UpdateProjectionMatrix()
	{
		switch (m_projection_mode)
		{
		case client_fw::eProjectionMode::kPerspective:
			m_projection_matrix = mat4::Perspective(math::ToRadian(m_field_of_view), m_aspect_ratio, m_near_z, m_far_z);
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
