#pragma once
#include "client/object/component/util/core/camera_component.h"

namespace client_fw
{
	class RenderTexture;

	class RenderCameraComponent : public CameraComponent
	{
	public:
		RenderCameraComponent(const std::string& name = "render camera component");
		virtual ~RenderCameraComponent() = default;

		virtual void UpdateViewMatrix() override;

	protected:
		WPtr<Actor> m_owner_controller;
		bool m_use_controller_rotation = false;

	public:
		void SetMainCamera();
		void SetOwnerController(const WPtr<Actor>& owner);
		bool IsUseControllerRotation() const { return m_use_controller_rotation; }
		void UseControllerRotation(bool use) { m_use_controller_rotation = use; }

	private:
		SPtr<RenderTexture> m_render_texture;

	public:
		const SPtr<RenderTexture>& GetRenderTexture() const { return m_render_texture; }
		// 카메라가 생성되면 카메라가 보는 장면을 그릴 Texture가 필요한데, 그 Texture를 뜻한다. 
		// 사용자가 직접적으로 호출할 필요는 없다.
		void SetRenderTexture(const SPtr<RenderTexture>& texture) { m_render_texture = texture; }

	protected:
		SPtr<RenderCameraComponent> SharedFromThis();
	};

}


