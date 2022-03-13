#pragma once

namespace client_fw
{
	enum class eCameraUsage;
	class CameraComponent;

	template<class T> class UploadBuffer;

	struct RSCameraData
	{
		Mat4 view_matrix;
		Mat4 projection_matrix;
		Mat4 view_projection_matrix;
		Vec3 camera_position;
		UINT final_texture_index;
		XMUINT4 gbuffer_texture_indices;
	};

	class CameraManager final
	{
	public:
		CameraManager();
		~CameraManager();

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;

		void Shutdown();
		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateMainCameraViewport(LONG width, LONG height);

		void Draw(ID3D12GraphicsCommandList* command_list, 
			std::function<void(ID3D12GraphicsCommandList*)>&& before_deferred_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& deferred_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& after_deferred_function);
		void DrawMainCameraForUI(ID3D12GraphicsCommandList* command_list);

		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		void CreateCameraResource(ID3D12Device* device);
		void UpdateCameraResource();
		void UpdateCameraResourceBeforeDraw();

	private:
		static CameraManager* s_camera_manager;
		std::map<eCameraUsage, UINT> m_camera_counts;
		std::map<eCameraUsage, std::vector<SPtr<CameraComponent>>> m_ready_cameras;
		std::map<eCameraUsage, std::vector<SPtr<CameraComponent>>> m_cameras;
		SPtr<CameraComponent> m_ready_main_camera;
		SPtr<CameraComponent> m_main_camera;
		UPtr<UploadBuffer<RSCameraData>> m_camera_data;

	public:
		static CameraManager& GetCameraManager() { return *s_camera_manager; }
		const std::vector<SPtr<CameraComponent>>& GetCameras(eCameraUsage usage) { return m_cameras[usage]; }
		const SPtr<CameraComponent> GetMainCamera() { return m_main_camera; }
		void SetMainCamera(const SPtr<CameraComponent>& camera_comp) { m_ready_main_camera = camera_comp; }
	};
}



