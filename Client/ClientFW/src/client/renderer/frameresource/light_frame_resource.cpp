#include "stdafx.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	LightFrameResource::LightFrameResource()
	{
		m_light_data = CreateUPtr<UploadBuffer<RSLightData>>();
	}

	LightFrameResource::~LightFrameResource()
	{
	}

	bool LightFrameResource::Initialize(ID3D12Device* device)
	{
		return true;
	}

	void LightFrameResource::Shutdown()
	{
		m_light_data->Shutdown();
	}
}
