#pragma once

namespace client_fw
{
	enum class eCameraUsage;
	class CameraComponent;

	class CameraManager final
	{
	public:
		CameraManager();
		~CameraManager();

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateMainCameraViewport(LONG left, LONG top, LONG width, LONG height);

		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		static CameraManager* s_camera_manager;
		std::map<eCameraUsage, UINT> m_camera_counts;
		std::map<eCameraUsage, std::vector<SPtr<CameraComponent>>> m_ready_cameras;
		std::map<eCameraUsage, std::vector<SPtr<CameraComponent>>> m_cameras;
		SPtr<CameraComponent> m_ready_main_camera;
		SPtr<CameraComponent> m_main_camera;

	public:
		static CameraManager& GetCameraManager() { return *s_camera_manager; }
		const std::vector<SPtr<CameraComponent>>& GetCameras(eCameraUsage usage) { return m_cameras[usage]; }
		const SPtr<CameraComponent> GetMainCamera() { return m_main_camera; }
		void SetMainCamera(const SPtr<CameraComponent>& camera_comp) { m_ready_main_camera = camera_comp; }
	};
}



