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
		// ī�޶� �����Ǹ� ī�޶� ���� ����� �׸� Texture�� �ʿ��ѵ�, �� Texture�� ���Ѵ�. 
		// ����ڰ� ���������� ȣ���� �ʿ�� ����.
		void SetRenderTexture(const SPtr<RenderTexture>& texture) { m_render_texture = texture; }

	protected:
		SPtr<RenderCameraComponent> SharedFromThis();
	};

}


