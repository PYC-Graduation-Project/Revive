#include "stdafx.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/actor/core/actor.h"
#include "client/renderer/core/render.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	CameraComponent::CameraComponent(const std::string& name, eCameraUsage usage)
		: SceneComponent(name), m_camera_state(eCameraState::kPaused)
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
		SceneComponent::UpdateWorldMatrix();

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
		m_inverse_view_matrix = mat4::Inverse(m_view_matrix);
		m_bounding_frustum.Transform(m_bf_projection, m_inverse_view_matrix);

		if (m_is_updated_viewport)
		{
			SetAspectRatio(static_cast<float>(m_viewport.width) / static_cast<float>(m_viewport.height));
			UpdateProjectionMatrix();
			m_is_updated_viewport = false;
		}
	}

	void CameraComponent::UpdateViewport(LONG left, LONG top, LONG width, LONG height)
	{
		m_viewport.left = left;
		m_viewport.width = width;
		m_viewport.top = top;
		m_viewport.height = height;
		m_is_updated_viewport = true;
		UpdateWorldMatrix();
	}

	void CameraComponent::UpdateProjectionMatrix()
	{
		switch (m_projection_mode)
		{
		case client_fw::eProjectionMode::kPerspective:
			m_projection_matrix = GetPerspectiveMatrix();
			break;
		case client_fw::eProjectionMode::kOrthographic:
			m_projection_matrix = GetOrthoMatrix();
			break;
		}
		m_bf_projection = BFrustum(m_projection_matrix);
	}

	bool CameraComponent::RegisterToRenderSystem()
	{
		if (Render::RegisterCameraComponent(SharedFromThis()))
			return true;
		return false;
	}

	void CameraComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterCameraComponent(SharedFromThis());
	}

	void CameraComponent::SetMainCamera()
	{
		Render::SetMainCamera(SharedFromThis());
	}

	void CameraComponent::SetOwnerController(const WPtr<Actor>& owner)
	{
		m_owner_controller = owner; 
		UpdateWorldMatrix();
	}

	Mat4 CameraComponent::GetPerspectiveMatrix() const
	{
		return mat4::Perspective(math::ToRadian(m_field_of_view), m_aspect_ratio, m_near_z, m_far_z);
	}

	Mat4 CameraComponent::GetOrthoMatrix() const
	{
		return  mat4::Ortho(static_cast<float>(m_viewport.left),
			static_cast<float>(m_viewport.left + m_viewport.width),
			static_cast<float>(m_viewport.top + m_viewport.height),
			static_cast<float>(m_viewport.top), m_near_z, m_far_z);
	}

	SPtr<CameraComponent> CameraComponent::SharedFromThis()
	{
		return std::static_pointer_cast<CameraComponent>(shared_from_this());
	}

}
