#include "stdafx.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/vertex.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	bool StaticMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_vertices.empty())
			return false;

		m_vertex_count = static_cast<UINT>(m_vertices.size());
		m_stride = sizeof(TextureLightVertex);

		if (m_indices.empty() == false)
		{
			m_is_draw_index = true;
			m_index_count = static_cast<UINT>(m_indices.size());
		}

		m_vertex_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, m_vertices.data(),
			m_stride * m_vertex_count, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_vertex_upload_buffer);

		m_vertex_buffer_view.BufferLocation = m_vertex_buffer->GetGPUVirtualAddress();
		m_vertex_buffer_view.SizeInBytes = m_vertex_count * m_stride;
		m_vertex_buffer_view.StrideInBytes = m_stride;

		if (m_vertex_buffer == nullptr)
		{
			LOG_ERROR("Could not create vertex buffer : {0}", m_asset_info.name);
			return false;
		}

		if (m_is_draw_index)
		{
			m_index_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, m_indices.data(),
				sizeof(UINT) * m_index_count, D3D12_RESOURCE_STATE_INDEX_BUFFER, m_index_upload_buffer);

			m_index_buffer_view.BufferLocation = m_index_buffer->GetGPUVirtualAddress();
			m_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
			m_index_buffer_view.SizeInBytes = sizeof(UINT) * m_index_count;

			if (m_index_buffer == nullptr)
			{
				LOG_ERROR("Could not create index buffer : {0}", m_asset_info.name);
				return false;
			}
		}
		
		m_vertices.clear();
		m_indices.clear();

		LOG_TRACE("Com");

		return true;
	}

	void StaticMesh::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		command_list->IASetPrimitiveTopology(m_primitive_topology);
		command_list->IASetVertexBuffers(m_slot, 1, &m_vertex_buffer_view);
	}

	void StaticMesh::Draw(ID3D12GraphicsCommandList* command_list, UINT instance_count, UINT material_index)
	{
		const auto& [count, start_location] = m_instance_info[material_index];
		if (m_is_draw_index)
			command_list->DrawIndexedInstanced(count, instance_count, start_location, 0, 0);
		else
			command_list->DrawInstanced(count, instance_count, start_location, 0);
	}

	void StaticMesh::SetVertices(std::vector<TextureLightVertex>&& vertices)
	{
		m_vertices = vertices;
	}

	void StaticMesh::SetVertices(std::vector<TextureLightVertex>&& vertices, std::vector<UINT>&& indices)
	{
		m_vertices = vertices;
		m_indices = indices;
	}

	void StaticMesh::AddInstanceInfo(InstanceInfo&& info)
	{
		m_instance_info.emplace_back(std::move(info));
	}
}