#include "stdafx.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/render_resource_frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/renderer/frameresource/sky_frame_resource.h"
#include "client/renderer/frameresource/mesh_frame_resource.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"
#include "client/renderer/frameresource/widget_frame_resource.h"
#include "client/renderer/frameresource/ui_frame_resource.h"

namespace client_fw
{
	FrameResource::FrameResource()
	{
		m_render_resource_frame_resource = CreateUPtr<RenderResourceFrameResource>();
		m_camera_frame_resource = CreateUPtr<CameraFrameResource>();
		m_light_frame_resource = CreateUPtr<LightFrameResource>();
	}

	FrameResource::~FrameResource()
	{
	}

	bool FrameResource::Initialize(ID3D12Device* device)
	{
		if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_command_allocator.GetAddressOf()))))
		{
			LOG_ERROR("Could not create command allocator");
			return false;
		}

		return true;
	}

	void FrameResource::Shutdown()
	{
		for (const auto& [shader_name, frame_resource] : m_local_light_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_ui_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_widget_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_billboard_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_static_mesh_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_skeletal_mesh_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_sky_frame_resource)
			frame_resource->Shutdown();
		m_light_frame_resource->Shutdown();
		m_camera_frame_resource->Shutdown();
		m_render_resource_frame_resource->Shutdown();
	}

	void FrameResource::CreateSkyFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_sky_frame_resource.emplace(shader_name, CreateUPtr<SkyFrameResource>());
		m_sky_frame_resource[shader_name]->Initialize(device);
	}

	void FrameResource::CreateStaticMeshFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_static_mesh_frame_resource.emplace(shader_name, CreateUPtr<StaticMeshFrameResource>());
		m_static_mesh_frame_resource[shader_name]->Initialize(device);
	}

	void FrameResource::CreateSkeletalMeshFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_skeletal_mesh_frame_resource.emplace(shader_name, CreateUPtr<SkeletalMeshFrameResource>());
		m_skeletal_mesh_frame_resource[shader_name]->Initialize(device);
	}

	void FrameResource::CreateBillboardFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_billboard_frame_resource.emplace(shader_name, CreateUPtr<BillboardFrameResource>());
		m_billboard_frame_resource[shader_name]->Initialize(device);
	}

	void FrameResource::CreateWidgetFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_widget_frame_resource.emplace(shader_name, CreateUPtr<WidgetFrameResource>());
		m_widget_frame_resource[shader_name]->Initialize(device);
	}

	void FrameResource::CreateLocalLightFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_local_light_frame_resource.emplace(shader_name, CreateUPtr<LocalLightFrameResource>());
		m_local_light_frame_resource[shader_name]->Initialize(device);
	}

	void FrameResource::CreateUserInterfaceFrameResource(ID3D12Device* device, const std::string& shader_name)
	{
		m_ui_frame_resource.emplace(shader_name, CreateUPtr<UserInterfaceFrameResource>());
		m_ui_frame_resource[shader_name]->Initialize(device);
	}
}
