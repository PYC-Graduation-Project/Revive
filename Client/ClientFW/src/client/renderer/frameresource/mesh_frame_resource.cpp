#include "stdafx.h"
#include "client/renderer/frameresource/mesh_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	MeshFrameResource::MeshFrameResource()
	{
	}

	MeshFrameResource::~MeshFrameResource()
	{
	}

	MeshesInstanceDrawInfo MeshFrameResource::GetMeshesInstanceDrawInfo()
	{
		MeshesInstanceDrawInfo info = m_meshes_instance_draw_info.front();
		m_meshes_instance_draw_info.pop();
		return info;
	}

	StaticMeshFrameResource::StaticMeshFrameResource()
	{
		m_instance_data = CreateUPtr<UploadBuffer<RSInstanceData>>(false);
	}

	StaticMeshFrameResource::~StaticMeshFrameResource()
	{
	}

	bool StaticMeshFrameResource::Initialize(ID3D12Device* device)
	{
		return true;
	}

	void StaticMeshFrameResource::Shutdown()
	{
		m_instance_data->Shutdown();
	}	
}