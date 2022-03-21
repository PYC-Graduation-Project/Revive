#include "stdafx.h"
#include "client/renderer/frameresource/mesh_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	MeshFrameResource::MeshFrameResource()
	{
		m_instance_data = CreateUPtr<UploadBuffer<RSInstanceData>>(false);
	}

	MeshFrameResource::~MeshFrameResource()
	{
	}

	bool MeshFrameResource::Initialize(ID3D12Device* device)
	{
		return true;
	}

	void MeshFrameResource::Shutdown()
	{
		m_instance_data->Shutdown();
	}

	MeshesInstanceDrawInfo MeshFrameResource::GetMeshesInstanceDrawInfo()
	{
		MeshesInstanceDrawInfo info = m_meshes_instance_draw_info.front();
		m_meshes_instance_draw_info.pop();
		return info;
	}
}