#include "stdafx.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"

namespace client_fw
{
	FrameResource::FrameResource()
	{
		m_billboard_frame_resource = CreateUPtr<BillboardFrameResource>();
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

		ret &= m_billboard_frame_resource->Initialize(device);

		return ret;
	}

	void FrameResource::Shutdown()
	{
		m_billboard_frame_resource->Shutdown();
	}
}
