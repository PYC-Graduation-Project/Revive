#include "stdafx.h"
#include "client/asset/primitive/billboard_primitive.h"
#include "client/asset/primitive/vertex.h"

namespace client_fw
{
	BillboardPrimitive::BillboardPrimitive()
	{
		m_vertex_info = CreateSPtr<UploadVertexInfo>();
	}

	bool BillboardPrimitive::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		return true;
	}

	bool BillboardPrimitive::Initialize(ID3D12Device* device)
	{
		m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		return true;
	}

	void BillboardPrimitive::Shutdown()
	{
		m_vertex_info->Shutdown();
	}

	void BillboardPrimitive::Update(ID3D12Device* device, UINT num_of_data)
	{
		m_vertex_info->CreateResource<BillboardVertex>(device, num_of_data);
	}

	void BillboardPrimitive::UpdateBillboardVertices(const std::vector<BillboardVertex>& vertices)
	{
		m_vertex_info->CopyData(vertices.data(), vertices.size());
	}

	void BillboardPrimitive::Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_vertex) const
	{
		command_list->IASetPrimitiveTopology(m_primitive_topology);
		m_vertex_info->Draw(command_list);
		command_list->DrawInstanced(num_of_vertex, 1, 0, 0);
	}
}