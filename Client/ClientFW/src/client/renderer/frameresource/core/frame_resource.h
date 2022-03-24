#pragma once

namespace client_fw
{
	class RenderResourceFrameResource;
	class CameraFrameResource;
	class StaticMeshFrameResource;
	class LightFrameResource;
	class BillboardFrameResource;
	class WidgetFrameResource;
	class UserInterfaceFrameResource;

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
		UPtr<StaticMeshFrameResource> m_static_mesh_frame_resource;
		UPtr<LightFrameResource> m_light_frame_resource;
		UPtr<BillboardFrameResource> m_billboard_frame_resource;
		UPtr<WidgetFrameResource> m_widget_frame_resource;
		UPtr<UserInterfaceFrameResource> m_ui_frame_resource;

	public:
		const UPtr<RenderResourceFrameResource>& GetRenderResourceFrameResource() const { return m_render_resource_frame_resource; }
		const UPtr<CameraFrameResource>& GetCameraFrameResource() const { return m_camera_frame_resource; }
		const UPtr<StaticMeshFrameResource>& GetStaticMeshFrameResource() const { return m_static_mesh_frame_resource; }
		const UPtr<LightFrameResource>& GetLightFrameResource() const { return m_light_frame_resource; }
		const UPtr<BillboardFrameResource>& GetBillboardFrameResource() const { return m_billboard_frame_resource; }
		const UPtr<WidgetFrameResource>& GetWidgetFrameResource() const { return m_widget_frame_resource; }
		const UPtr<UserInterfaceFrameResource>& GetUserInterfaceFrameResource() const { return m_ui_frame_resource; }
	};
}



