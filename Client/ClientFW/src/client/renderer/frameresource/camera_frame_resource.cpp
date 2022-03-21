#include "stdafx.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	CameraFrameResource::CameraFrameResource()
	{
		m_camera_data = CreateUPtr<UploadBuffer<RSCameraData>>(true);
	}

	CameraFrameResource::~CameraFrameResource()
	{
	}

	bool CameraFrameResource::Initialize(ID3D12Device* device)
	{
		return true;
	}

	void CameraFrameResource::Shutdown()
	{
		m_camera_data->Shutdown();
	}
}
