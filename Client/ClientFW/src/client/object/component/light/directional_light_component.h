#pragma once
#include "client/object/component/light/core/light_component.h"

namespace client_fw
{
	class RenderCameraComponent;
	class ShadowCascadeCameraComponent;

	struct CascadeShadowForRenderCamera
	{
		WPtr<RenderCameraComponent> render_camera;
		SPtr<ShadowCascadeCameraComponent> shadow_cascade_camera;
	};

	class DirectionalLightComponent : public LightComponent
	{
	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent() = default;

	private:
		std::vector<CascadeShadowForRenderCamera> m_cascade_shadows_camera;

	public:
		const std::vector<CascadeShadowForRenderCamera>& GetCascadeShadowsCamera() const { return m_cascade_shadows_camera; }
		void RegisterCascadeAndRenderCamera(const WPtr<RenderCameraComponent>& render_camera_comp,
			const SPtr<ShadowCascadeCameraComponent>& cascade_camera_comp);
		void UnregisterRenderCamera(const WPtr<RenderCameraComponent>& render_camera_comp);

	};
}


