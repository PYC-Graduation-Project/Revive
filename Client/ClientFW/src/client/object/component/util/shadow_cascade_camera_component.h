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
		// 카메라가 생성되면 카메라가 보는 장면을 그릴 Texture가 필요한데, 그 Texture를 뜻한다. 
		// 사용자가 직접적으로 호출할 필요는 없다.
		void SetShadowArrayTexture(const SPtr<ShadowArrayTexture>& texture) { m_shadow_array_texture = texture; }
	};
}



