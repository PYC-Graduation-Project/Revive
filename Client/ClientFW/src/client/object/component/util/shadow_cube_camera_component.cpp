#include "stdafx.h"
#include "client/object/component/util/shadow_cube_camera_component.h"

namespace client_fw
{
	ShadowCubeCameraComponent::ShadowCubeCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kShadowCube, name)
	{
		SetFieldOfView(90.0f);
	}

	bool ShadowCubeCameraComponent::Initialize()
	{
		bool ret = CameraComponent::Initialize();
		return ret;
	}

	void ShadowCubeCameraComponent::UpdateViewMatrix()
	{
		m_cube_view_matrixs[0] = mat4::LookAt(GetWorldPosition(), GetWorldPosition() + vec3::AXIS_X, vec3::AXIS_Y);
		m_cube_view_matrixs[1] = mat4::LookAt(GetWorldPosition(), GetWorldPosition() + vec3::NEG_AXIS_X, vec3::AXIS_Y);
		m_cube_view_matrixs[2] = mat4::LookAt(GetWorldPosition(), GetWorldPosition() + vec3::AXIS_Y, vec3::NEG_AXIS_Z);
		m_cube_view_matrixs[3] = mat4::LookAt(GetWorldPosition(), GetWorldPosition() + vec3::NEG_AXIS_Y, vec3::AXIS_Z);
		m_cube_view_matrixs[4] = mat4::LookAt(GetWorldPosition(), GetWorldPosition() + vec3::AXIS_Z, vec3::AXIS_Y);
		m_cube_view_matrixs[5] = mat4::LookAt(GetWorldPosition(), GetWorldPosition() + vec3::NEG_AXIS_Z, vec3::AXIS_Y);
	}
}
