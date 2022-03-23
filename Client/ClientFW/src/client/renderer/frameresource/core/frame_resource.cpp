#include "stdafx.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/renderer/frameresource/mesh_frame_resource.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"
#include "client/renderer/frameresource/widget_frame_resource.h"
#include "client/renderer/frameresource/ui_frame_resource.h"

namespace client_fw
{
	FrameResource::FrameResource()
	{
		m_camera_frame_resource = CreateUPtr<CameraFrameResource>();
		m_static_mesh_frame_resource = CreateUPtr<StaticMeshFrameResource>();
		m_skeletal_mesh_frame_resource = CreateUPtr<SkeletalMeshFrameResource>();
		m_billboard_frame_resource = CreateUPtr<BillboardFrameResource>();
		m_widget_frame_resource = CreateUPtr<WidgetFrameResource>();
		m_ui_frame_resource = CreateUPtr<UserInterfaceFrameResource>();
	}

	FrameResource::~FrameResource()
	{
	}

	bool FrameResource::Initialize(ID3D12Device* device)
	{
		bool ret = true;

		if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_command_allocator.GetAddressOf()))))
		{
			LOG_ERROR("Could not create command allocator");
			return false;
		}

		ret &= m_camera_frame_resource->Initialize(device);
		ret &= m_static_mesh_frame_resource->Initialize(device);
		ret &= m_skeletal_mesh_frame_resource->Initialize(device);
		ret &= m_billboard_frame_resource->Initialize(device);
		ret &= m_widget_frame_resource->Initialize(device);
		ret &= m_ui_frame_resource->Initialize(device);

		return ret;
	}

	void FrameResource::Shutdown()
	{
		m_ui_frame_resource->Shutdown();
		m_widget_frame_resource->Shutdown();
		m_billboard_frame_resource->Shutdown();
		m_static_mesh_frame_resource->Shutdown();
		m_skeletal_mesh_frame_resource->Shutdown();
		m_camera_frame_resource->Shutdown();
	}
}
