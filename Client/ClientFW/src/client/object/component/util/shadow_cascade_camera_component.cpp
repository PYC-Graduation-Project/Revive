#include "stdafx.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"

namespace client_fw
{
	ShadowCascadeCameraComponent::ShadowCascadeCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kShadowCascade, name)
	{
	}

	bool ShadowCascadeCameraComponent::Initialize()
	{
		bool ret = CameraComponent::Initialize();
		SetViewport(Viewport{ 0, 0, 4096, 4096 });
		return true;
	}

	void ShadowCascadeCameraComponent::UpdateViewMatrix()
	{
	}

	void ShadowCascadeCameraComponent::UpdateProjectionMatrix()
	{
	}
}
