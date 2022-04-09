#pragma once

namespace client_fw
{
	enum class eCameraUsage;
	class CameraComponent;
	class RenderCameraComponent;

	// 카메라의 정보를 GPU에서 사용하기 위해 Camera를 관리하는 클래스
	class CameraManager final
	{
	public:
		CameraManager();
		~CameraManager();

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;

		void Shutdown();
		void Update(ID3D12Device* device, std::function<void(ID3D12Device*)>&& update_shader_function);
		void UpdateMainCameraViewport(LONG width, LONG height);

		void Draw(ID3D12GraphicsCommandList* command_list, 
			std::function<void(ID3D12GraphicsCommandList*)>&& before_deferred_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& deferred_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& after_deferred_function);
		void DrawMainCameraForUI(ID3D12GraphicsCommandList* command_list);

		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		void UpdateCameraResource(ID3D12Device* device, std::function<void(ID3D12Device*)>&& update_shader_function);

	private:
		static CameraManager* s_camera_manager;

		std::vector<SPtr<RenderCameraComponent>> m_ready_render_cameras;
		std::vector<SPtr<RenderCameraComponent>> m_wait_resource_render_cameras;
		std::vector<SPtr<RenderCameraComponent>> m_render_cameras;

		SPtr<RenderCameraComponent> m_ready_main_camera;
		SPtr<RenderCameraComponent> m_main_camera;

	public:
		static CameraManager& GetCameraManager() { return *s_camera_manager; }
		const SPtr<RenderCameraComponent> GetMainCamera() { return m_main_camera; }
		void SetMainCamera(const SPtr<RenderCameraComponent>& camera_comp) { m_ready_main_camera = camera_comp; }
	};
}



