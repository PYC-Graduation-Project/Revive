#include "stdafx.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/vertex.h"
#include "client/asset/mesh/material.h"
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/collision/mesh_collision_manager.h"


namespace client_fw
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Shutdown()
	{
		for (const auto& data : m_material_index_data)
			data->Shutdown();
	}

	void Mesh::PostDraw(ID3D12GraphicsCommandList* command_list)
	{
		std::fill(m_lod_mesh_counts.begin(), m_lod_mesh_counts.end(), 0);
	}

	void Mesh::CreateDataForLodMesh(UINT lod)
	{
		if (lod == m_lod_count)
		{
			m_lod_mesh_counts.emplace_back(0);
			m_vertex_buffer_blobs.emplace_back(ComPtr<ID3DBlob>());
			m_vertex_buffers.emplace_back(ComPtr<ID3D12Resource>());
			m_vertex_upload_buffers.emplace_back(ComPtr<ID3D12Resource>());
			m_vertex_buffer_views.emplace_back(D3D12_VERTEX_BUFFER_VIEW());
			m_index_buffer_blobs.emplace_back(ComPtr<ID3DBlob>());
			m_index_buffers.emplace_back(ComPtr<ID3D12Resource>());
			m_index_upload_buffers.emplace_back(ComPtr<ID3D12Resource>());
			m_index_buffer_views.emplace_back(D3D12_INDEX_BUFFER_VIEW());

			m_materials.emplace_back(std::vector<SPtr<Material>>());

			++m_lod_count;
		}
	}

	bool StaticMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (UINT i = 0; i < m_lod_count; ++i)
		{
			auto vertices = reinterpret_cast<TextureLightVertex*>(m_vertex_buffer_blobs.at(i)->GetBufferPointer());
			auto vertices_size = static_cast<UINT>(m_vertex_buffer_blobs.at(i)->GetBufferSize());

			m_vertex_buffers.at(i) = D3DUtil::CreateDefaultBuffer(device, command_list, vertices, vertices_size,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_vertex_upload_buffers.at(i));

			m_vertex_buffer_views.at(i).BufferLocation = m_vertex_buffers.at(i)->GetGPUVirtualAddress();
			m_vertex_buffer_views.at(i).SizeInBytes = vertices_size;
			m_vertex_buffer_views.at(i).StrideInBytes = sizeof(TextureLightVertex);

			if (m_vertex_buffers.at(i) == nullptr)
			{
				LOG_ERROR("Could not create vertex buffer : {0}", m_asset_info.name);
				return false;
			}

			if (m_index_buffer_blobs.at(i) != nullptr)
			{
				m_is_draw_index = true;

				auto indices = reinterpret_cast<UINT*>(m_index_buffer_blobs.at(i)->GetBufferPointer());
				auto indices_size = static_cast<UINT>(m_index_buffer_blobs.at(i)->GetBufferSize());

				m_index_buffers.at(i) = D3DUtil::CreateDefaultBuffer(device, command_list, indices, indices_size,
					D3D12_RESOURCE_STATE_INDEX_BUFFER, m_index_upload_buffers.at(i));

				m_index_buffer_views.at(i).BufferLocation = m_index_buffers.at(i)->GetGPUVirtualAddress();
				m_index_buffer_views.at(i).Format = DXGI_FORMAT_R32_UINT;
				m_index_buffer_views.at(i).SizeInBytes = indices_size;

				if (m_index_buffers.at(i) == nullptr)
				{
					LOG_ERROR("Could not create index buffer : {0}", m_asset_info.name);
					return false;
				}
			}

			m_material_index_data.emplace_back(CreateUPtr<UploadBuffer<RSMaterialIndexData>>(true));
			UINT mat_size = static_cast<UINT>(m_materials.at(i).size());
			m_material_index_data.at(i)->CreateResource(device, mat_size);
			for (UINT index = 0; index < mat_size; ++index)
				m_material_index_data.at(i)->CopyData(index, RSMaterialIndexData{ m_materials.at(i)[index]->GetResourceIndex() });
		}

		return true;
	}

	void StaticMesh::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		command_list->IASetPrimitiveTopology(m_primitive_topology);
	}

	void StaticMesh::Draw(ID3D12GraphicsCommandList* command_list, UINT lod)
	{
		command_list->IASetVertexBuffers(m_slot, 1, &m_vertex_buffer_views.at(lod));
		if (m_is_draw_index)
			command_list->IASetIndexBuffer(&m_index_buffer_views.at(lod));

		for (size_t mat_index = 0; mat_index < m_materials.at(lod).size(); ++mat_index)
		{
			command_list->SetGraphicsRootConstantBufferView(0, m_material_index_data.at(lod)->GetResource()->GetGPUVirtualAddress() +
				mat_index * m_material_index_data.at(lod)->GetByteSize());
			const auto& [count, start_location] = m_instance_info.at(lod)[mat_index];
			if (m_is_draw_index)
				command_list->DrawIndexedInstanced(count, m_lod_mesh_counts.at(lod), start_location, 0, 0);
			else
				command_list->DrawInstanced(count, m_lod_mesh_counts.at(lod), start_location, 0);
		}
	}

	void StaticMesh::CreateDataForLodMesh(UINT lod)
	{
		if (lod == m_lod_count)
		{
			Mesh::CreateDataForLodMesh(lod);
			m_instance_info.push_back(std::vector<InstanceInfo>());
		}
	}

	void StaticMesh::CreateCollision(const SPtr<MeshComponent> mesh_comp)
	{
		mesh_comp->SetMeshCollisionManager(
			CreateSPtr<StaticMeshCollisionManager>(mesh_comp, m_bounding_tree, mesh_comp->GetOrientedBox()));
	}

	ComPtr<ID3DBlob> Mesh::GetVertexBufferBlob(UINT lod)
	{
		if (lod < m_lod_count)
			return m_vertex_buffer_blobs.at(lod);
		else
		{
			LOG_ERROR("{0}-lod index out of range : {1}", m_asset_info.name, lod);
			return nullptr;
		}
	}

	bool Mesh::CreateVertexBufferBlob(UINT lod, UINT size)
	{
		return SUCCEEDED(D3DCreateBlob(size, m_vertex_buffer_blobs.at(lod).GetAddressOf()));
	}

	ComPtr<ID3DBlob> Mesh::GetIndexBufferBlob(UINT lod)
	{
		if (lod < m_lod_count)
			return m_index_buffer_blobs.at(lod);
		else
		{
			LOG_ERROR("{0}-lod index out of range : {1}", m_asset_info.name, lod);
			return nullptr;
		}
	}

	bool Mesh::CreateIndexBufferBlob(UINT lod, UINT size)
	{
		return SUCCEEDED(D3DCreateBlob(size, m_index_buffer_blobs.at(lod).GetAddressOf()));
	}

	void StaticMesh::AddInstanceInfo(UINT lod, InstanceInfo&& info)
	{
		if (lod < m_lod_count)
			m_instance_info.at(lod).emplace_back(std::move(info));
	}
	
}