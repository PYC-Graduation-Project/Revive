#pragma once
#include "client/object/component/core/component.h"

namespace client_fw
{
	enum class eProjectionMode
	{
		kPerspective, kOrthographic
	};

	enum class eCameraState
	{
		kActive, kPaused
	};

	enum class eCameraUsage
	{
		kBasic, kLight,
	};

	class CameraComponent : public Component, public std::enable_shared_from_this<CameraComponent>
	{
	public:
		CameraComponent(const std::string& name = "camera component",
			eCameraUsage usage = eCameraUsage::kBasic);
		virtual ~CameraComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void UpdateWorldMatrix();
		virtual void UpdateProjectionMatrix();

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();

	protected:
		eCameraState m_camera_state;
		eCameraUsage m_camera_usage;
		eProjectionMode m_projection_mode;
		Mat4 m_view_matrix;
		Mat4 m_projection_matrix;
		float m_aspect_ratio = 1.777778f;
		float m_field_of_view = 45.0f;
		float m_near_z = 1.01f;
		float m_far_z = 100000.0f;

	public:
		eCameraState GetCameraState() const { return m_camera_state; }
		void SetActive() { m_camera_state = eCameraState::kActive; }
		void SetPaused() { m_camera_state = eCameraState::kPaused; }
		eCameraUsage GetCameraUsage() const { return m_camera_usage; }
		const Mat4& GetViewMatrix() const { return m_view_matrix; }
		const Mat4& GetProjectionMatrix() const { return m_projection_matrix; }
		void SetAspectRatio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }
		void SetFieldOfView(float fov) { m_field_of_view = fov; }
		void SetNearZ(float near_z) { m_near_z = near_z; }
		void SetFarZ(float far_z) { m_far_z = far_z; }
	};
}


