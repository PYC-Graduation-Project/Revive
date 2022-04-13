#pragma once
#include "client/object/component/util/core/camera_component.h"

namespace client_fw
{
	class ShadowCubeTexture;

	class ShadowCubeCameraComponent : public CameraComponent
	{
	public:
		ShadowCubeCameraComponent(const std::string& name = "shadow cube camera component");
		virtual ~ShadowCubeCameraComponent() = default;

		virtual bool Initialize() override;
		virtual void UpdateViewMatrix() override;

	private:
		SPtr<ShadowCubeTexture> m_shadow_cube_texture;
		std::array<Mat4, 6> m_cube_view_matrixs;

	public:
		const SPtr<ShadowCubeTexture>& GetShadowCubeTexture() const { return m_shadow_cube_texture; }
		// ī�޶� �����Ǹ� ī�޶� ���� ����� �׸� Texture�� �ʿ��ѵ�, �� Texture�� ���Ѵ�. 
		// ����ڰ� ���������� ȣ���� �ʿ�� ����.
		void SetShadowCubeTexture(const SPtr<ShadowCubeTexture>& texture) { m_shadow_cube_texture = texture; }

		const std::array<Mat4, 6>& GetCubeViewMatrixs() const { return m_cube_view_matrixs; }
		float GetFarZ() const { return m_far_z; }
	};


}
