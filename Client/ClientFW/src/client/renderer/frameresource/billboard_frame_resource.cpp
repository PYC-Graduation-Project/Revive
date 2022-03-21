#include "stdafx.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	BillboardFrameResource::BillboardFrameResource()
	{
		m_texture_billboard_primitive = CreateUPtr<UploadPrimitive<BillboardVertex>>();
		m_material_billboard_primitive = CreateUPtr<UploadPrimitive<BillboardVertex>>();
	}
	BillboardFrameResource::~BillboardFrameResource()
	{
	}

	bool BillboardFrameResource::Initialize(ID3D12Device* device)
	{
		bool ret = true;
		ret &= m_texture_billboard_primitive->Initialize(device);
		ret &= m_material_billboard_primitive->Initialize(device);
		return ret;
	}

	void BillboardFrameResource::Shutdown()
	{
		m_material_billboard_primitive->Shutdown();
		m_texture_billboard_primitive->Shutdown();
	}

	BillboardDrawInfo BillboardFrameResource::GetTextureBillboardDrawInfo()
	{
		BillboardDrawInfo info = m_texture_draw_info.front();
		m_texture_draw_info.pop();
		return info;
	}

	BillboardDrawInfo BillboardFrameResource::GetMaterialBillboardDrawInfo()
	{
		BillboardDrawInfo info = m_material_draw_info.front();
		m_material_draw_info.pop();
		return info;
	}
}
