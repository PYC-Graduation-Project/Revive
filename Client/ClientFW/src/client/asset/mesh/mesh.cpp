#include "stdafx.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/vertex.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	bool StaticMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{

		auto vertices = reinterpret_cast<TextureLightVertex*>(m_vertex_buffer_blob->GetBufferPointer());
		auto vertices_size = static_cast<UINT>(m_vertex_buffer_blob->GetBufferSize());

		m_vertex_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, vertices, vertices_size,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_vertex_upload_buffer);

		m_vertex_buffer_view.BufferLocation = m_vertex_buffer->GetGPUVirtualAddress();
		m_vertex_buffer_view.SizeInBytes = vertices_size;
		m_vertex_buffer_view.StrideInBytes = sizeof(TextureLightVertex);

		if (m_vertex_buffer == nullptr)
		{
			LOG_ERROR("Could not create vertex buffer : {0}", m_asset_info.name);
			return false;
		}

		if (m_index_buffer_blob != nullptr)
		{
			m_is_draw_index = true;

			auto indices = reinterpret_cast<UINT*>(m_index_buffer_blob->GetBufferPointer());
			auto indices_size = static_cast<UINT>(m_index_buffer_blob->GetBufferSize());

			m_index_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, indices, indices_size,
				D3D12_RESOURCE_STATE_INDEX_BUFFER, m_index_upload_buffer);

			m_index_buffer_view.BufferLocation = m_index_buffer->GetGPUVirtualAddress();
			m_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
			m_index_buffer_view.SizeInBytes = indices_size;

			if (m_index_buffer == nullptr)
			{
				LOG_ERROR("Could not create index buffer : {0}", m_asset_info.name);
				return false;
			}
		}

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

	void StaticMesh::AddInstanceInfo(InstanceInfo&& info)
	{
		m_instance_info.emplace_back(std::move(info));
	}
	void Skeleton::SetChild(SPtr<Skeleton>& child)
	{
		if (child)
		{
			//child->m_parent = this;
		}
		if (m_child)
		{
			if (child) child->m_sibling = m_child->m_sibling;
			m_child->m_sibling = child;
		}
		else
		{
			m_child = child;
		}
	}

	bool SkeletalMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		std::cout << "초기화합니다" << std::endl;
		return false;
	}

	void SkeletalMesh::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		std::cout << "미리그립니다" << std::endl;

	}

	void SkeletalMesh::Draw(ID3D12GraphicsCommandList* command_list, UINT instance_count, UINT material_index)
	{
		std::cout << "그립니다" << std::endl;

	}
}