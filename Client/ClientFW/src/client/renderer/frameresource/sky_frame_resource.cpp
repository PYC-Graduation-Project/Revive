#include "stdafx.h"
#include "client/renderer/frameresource/sky_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	SkyFrameResource::SkyFrameResource()
	{
		m_cube_texture_index_data = CreateUPtr<UploadBuffer<RSCubeTextureIndexData>>(true);
	}

	SkyFrameResource::~SkyFrameResource()
	{
	}

	bool SkyFrameResource::Initialize(ID3D12Device* device)
	{
		m_cube_texture_index_data->CreateResource(device, 1);
		return true;
	}

	void SkyFrameResource::Shutdown()
	{
		m_cube_texture_index_data->Shutdown();
	}
}