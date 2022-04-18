#pragma once
#include "client/object/component/util/core/camera_component.h"

namespace client_fw
{
	class ShadowArrayTexture;

	class ShadowCascadeCameraComponent : public CameraComponent
	{
	public:
		ShadowCascadeCameraComponent(const std::string& name = "shadow cascade camera component");
		virtual ~ShadowCascadeCameraComponent() = default;

		virtual bool Initialize() override;
		virtual void UpdateViewMatrix() override;
		virtual void UpdateProjectionMatrix();

	private:
		SPtr<ShadowArrayTexture> m_shadow_array_texture;

	public:
		const SPtr<ShadowArrayTexture>& GetShadowArrayTexture() const { return m_shadow_array_texture; }
		// ī�޶� �����Ǹ� ī�޶� ���� ����� �׸� Texture�� �ʿ��ѵ�, �� Texture�� ���Ѵ�. 
		// ����ڰ� ���������� ȣ���� �ʿ�� ����.
		void SetShadowArrayTexture(const SPtr<ShadowArrayTexture>& texture) { m_shadow_array_texture = texture; }
	};
}



