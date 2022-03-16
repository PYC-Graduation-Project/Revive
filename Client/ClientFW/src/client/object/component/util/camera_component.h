#pragma once
#include "client/object/component/core/scene_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class Actor;
	class RenderTexture;

	enum class eProjectionMode
	{
		kPerspective, kOrthographic
	};

	enum class eCameraState
	{
		kActive, kPaused
	};

	//아마 Light는 LightComponent에 넣을 것 같다.
	//일단 Light를 넣을 때 다시 생각해 볼 생각이라 수정은 하지 않겠다.
	enum class eCameraUsage
	{
		kBasic, kLight,
	};

	struct Viewport
	{
		LONG left = 0;
		LONG top = 0;
		LONG width = 1366;
		LONG height = 768;
	};

	class CameraComponent : public SceneComponent
	{
	public:
		CameraComponent(const std::string& name = "camera component",
			eCameraUsage usage = eCameraUsage::kBasic);
		virtual ~CameraComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void UpdateWorldMatrix() override;
		virtual void UpdateViewport(LONG left, LONG top, LONG width, LONG height);
		virtual void UpdateProjectionMatrix();

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();

	protected:
		WPtr<Actor> m_owner_controller;
		eCameraState m_camera_state;
		eCameraUsage m_camera_usage;
		eProjectionMode m_projection_mode;
		Viewport m_viewport;
		bool m_is_updated_viewport = true;
		Mat4 m_view_matrix;
		Mat4 m_inverse_view_matrix;
		Mat4 m_projection_matrix;
		float m_aspect_ratio = 1.777778f;
		float m_field_of_view = 45.0f;
		float m_near_z = 1.01f;
		float m_far_z = 100000.0f;
		BFrustum m_bf_projection;
		BFrustum m_bounding_frustum;

	public:
		void SetMainCamera();
		void SetOwnerController(const WPtr<Actor>& owner);
		eCameraState GetCameraState() const { return m_camera_state; }
		void SetActive() { m_camera_state = eCameraState::kActive; }
		void SetPaused() { m_camera_state = eCameraState::kPaused; }
		void SetProjectionMode(eProjectionMode mode) { m_projection_mode = mode; }
		eCameraUsage GetCameraUsage() const { return m_camera_usage; }
		const Viewport& GetViewport() const { return m_viewport; }
		void SetViewport(const Viewport& viewport) { m_viewport = viewport; m_is_updated_viewport = true; }
		const Mat4& GetViewMatrix() const { return m_view_matrix; }
		const Mat4& GetProjectionMatrix() const { return m_projection_matrix; }
		Mat4 GetPerspectiveMatrix() const;
		Mat4 GetOrthoMatrix() const;
		void SetAspectRatio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }
		void SetFieldOfView(float fov) { m_field_of_view = fov; }
		void SetNearZ(float near_z) { m_near_z = near_z; }
		void SetFarZ(float far_z) { m_far_z = far_z; }
		const BFrustum& GetBoudingFrustum() const { return m_bounding_frustum; }

	private:
		SPtr<RenderTexture> m_render_texture;

	public:
		const SPtr<RenderTexture>& GetRenderTexture() const { return m_render_texture; }
		void SetRenderTexture(const SPtr<RenderTexture>& texture) { m_render_texture = texture; }

	protected:
		SPtr<CameraComponent> SharedFromThis();

	};
}



