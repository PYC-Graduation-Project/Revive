#pragma once

namespace client_fw
{
	class RenderResourceFrameResource;
	class CameraFrameResource;
	class SkyFrameResource;
	class StaticMeshFrameResource;
	class LightFrameResource;
	class BillboardFrameResource;
	class WidgetFrameResource;
	class UserInterfaceFrameResource;

	//GPU가 사용할 데이터들을 관리하는 곳
	//CPU와 GPU의 병렬성을 위해서 필요하다.
	class FrameResource
	{
	public:
		FrameResource();
		~FrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		ComPtr<ID3D12CommandAllocator> m_command_allocator;
		UINT64 m_fence = 0;

	public:
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const { return m_command_allocator; }
		UINT64 GetFence() const { return m_fence; }
		void SetFence(UINT64 value) { m_fence = value; }

	private:
		UPtr<RenderResourceFrameResource> m_render_resource_frame_resource;
		UPtr<CameraFrameResource> m_camera_frame_resource;
		UPtr<LightFrameResource> m_light_frame_resource;

	public:
		const UPtr<RenderResourceFrameResource>& GetRenderResourceFrameResource() const { return m_render_resource_frame_resource; }
		const UPtr<CameraFrameResource>& GetCameraFrameResource() const { return m_camera_frame_resource; }
		const UPtr<LightFrameResource>& GetLightFrameResource() const { return m_light_frame_resource; }

	private:
		std::map<std::string, UPtr<SkyFrameResource>> m_sky_frame_resource;
		std::map<std::string, UPtr<StaticMeshFrameResource>> m_static_mesh_frame_resource;
		std::map<std::string, UPtr<BillboardFrameResource>> m_billboard_frame_resource;
		std::map<std::string, UPtr<WidgetFrameResource>> m_widget_frame_resource;
		std::map<std::string, UPtr<UserInterfaceFrameResource>> m_ui_frame_resource;

	public:
		void CreateSkyFrameResource(ID3D12Device* device, const std::string& shader_name);
		void CreateStaticMeshFrameResource(ID3D12Device* device, const std::string& shader_name);
		void CreateBillboardFrameResource(ID3D12Device* device, const std::string& shader_name);
		void CreateWidgetFrameResource(ID3D12Device* device, const std::string& shader_name);
		void CreateUserInterfaceFrameResource(ID3D12Device* device, const std::string& shader_name);

		const UPtr<SkyFrameResource>& GetSkyFrameResource(const std::string& shader_name) const
		{ return m_sky_frame_resource.at(shader_name); }
		const UPtr<StaticMeshFrameResource>& GetStaticMeshFrameResource(const std::string& shader_name) const 
		{ return m_static_mesh_frame_resource.at(shader_name); }
		const UPtr<BillboardFrameResource>& GetBillboardFrameResource(const std::string& shader_name) const 
		{ return m_billboard_frame_resource.at(shader_name); }
		const UPtr<WidgetFrameResource>& GetWidgetFrameResource(const std::string& shader_name) const 
		{ return m_widget_frame_resource.at(shader_name); }
		const UPtr<UserInterfaceFrameResource>& GetUserInterfaceFrameResource(const std::string& shader_name) const 
		{ return m_ui_frame_resource.at(shader_name); }
	};
}



