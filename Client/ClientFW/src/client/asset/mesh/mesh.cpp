#include "stdafx.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/primitive/vertex.h"
#include "client/asset/material/material.h"
#include "client/asset/bone/skeleton.h"
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	Mesh::Mesh() : Primitive()
	{
		m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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
			m_vertex_infos.emplace_back(CreateSPtr<VertexInfo>());
			m_index_infos.emplace_back(CreateSPtr<IndexInfo>());

			m_instance_info.emplace_back(std::vector<InstanceInfo>());
			m_materials.emplace_back(std::vector<SPtr<Material>>());

			++m_lod_count;
		}
	}

	void Mesh::AddInstanceInfo(UINT lod, InstanceInfo&& info)
	{
		if (lod < m_lod_count)
			m_instance_info.at(lod).emplace_back(std::move(info));
	}

	bool StaticMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (UINT i = 0; i < m_lod_count; ++i)
		{
			if (m_vertex_infos.at(i)->Initialize<TextureLightVertex>(device, command_list) == false)
			{
				LOG_ERROR("Could not create vertex buffer : {0}", m_asset_info.name);
				return false;
			}

			if (m_is_draw_index)
			{
				if (m_index_infos.at(i)->Initialize(device, command_list) == false)
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

	void StaticMesh::PreDraw(ID3D12GraphicsCommandList* command_list) const
	{
		command_list->IASetPrimitiveTopology(m_primitive_topology);
	}

	void StaticMesh::Draw(ID3D12GraphicsCommandList* command_list, UINT lod) const
	{
		m_vertex_infos.at(lod)->Draw(command_list);
		if (m_is_draw_index)
			m_index_infos.at(lod)->Draw(command_list);
			

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

	

	bool SkeletalMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (UINT i = 0; i < m_lod_count; ++i)
		{
			if (m_vertex_infos.at(i)->Initialize<TextureLightVertex>(device, command_list) == false)
			{
				LOG_ERROR("Could not create vertex buffer : {0}", m_asset_info.name);
				return false;
			}

			if (m_is_draw_index)
			{
				if (m_index_infos.at(i)->Initialize(device, command_list) == false)
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
			

			auto bone_indices = (m_bone_data->bone_indices.data());
			auto bone_indices_size = static_cast<UINT>(m_bone_data->bone_indices.size());

			m_bone_index_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, bone_indices, bone_indices_size,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_bone_index_upload_buffer);

			m_bone_index_buffer_view.BufferLocation = m_bone_index_buffer->GetGPUVirtualAddress();
			m_bone_index_buffer_view.SizeInBytes = bone_indices_size;
			m_bone_index_buffer_view.StrideInBytes = sizeof(IVec4);

			auto bone_weights = (m_bone_data->bone_weights.data());
			auto bone_weights_size = static_cast<UINT>(m_bone_data->bone_weights.size());

			m_bone_weight_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, bone_weights, bone_weights_size,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_bone_weight_upload_buffer);

			m_bone_weight_buffer_view.BufferLocation = m_bone_weight_buffer->GetGPUVirtualAddress();
			m_bone_weight_buffer_view.SizeInBytes = bone_weights_size;
			m_bone_weight_buffer_view.StrideInBytes = sizeof(Vec4);

			
			
			
			
			
			AnimationData temp_data;
			for (UINT i = 0; i < m_bone_data->bone_offsets.size(); ++i)
			{
				temp_data.mat128.at(i) = m_bone_data->bone_offsets.at(i);
			}
			m_bone_offset_data.emplace_back(CreateUPtr<UploadBuffer<AnimationData>>(true));
			m_bone_offset_data.at(i)->CreateResource(device, 128);
			m_bone_offset_data.at(i)->CopyData(0, AnimationData{ temp_data.mat128 });

			UINT trans_size = static_cast<UINT>(m_bone_data->bone_names.size());
			m_cache_skeleton.resize(trans_size);

			SPtr<Skeleton> skel = nullptr;
			
			for (UINT i = 0; i < trans_size; ++i)
			{
				skel = m_skeleton->FindBone(m_bone_data->bone_names.at(i));
				temp_data.mat128.at(i) = skel->GetWorld();
				m_cache_skeleton.at(i) = skel;
			}
			m_bone_trans_data.emplace_back(CreateUPtr<UploadBuffer<AnimationData>>(true));
			m_bone_trans_data.at(i)->CreateResource(device, 128);
			m_bone_trans_data.at(i)->CopyData(0, AnimationData{ temp_data.mat128 });
			
			

		}
		

		return true;
	}

	void SkeletalMesh::PreDraw(ID3D12GraphicsCommandList* command_list) const
	{
		command_list->IASetPrimitiveTopology(m_primitive_topology);
	}

	void SkeletalMesh::Draw(ID3D12GraphicsCommandList* command_list, UINT lod) const
	{
		m_vertex_infos.at(lod)->Draw(command_list);
		if (m_is_draw_index)
			m_index_infos.at(lod)->Draw(command_list);
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_views[3] = { m_vertex_infos.at(lod)->m_vertex_buffer_view,m_bone_index_buffer_view ,m_bone_weight_buffer_view };
		
		command_list->IASetVertexBuffers(0, 3, vertex_buffer_views);
		
		UpdateBoneTransform();
		

		command_list->SetGraphicsRootConstantBufferView(5, m_bone_offset_data.at(lod)->GetResource()->GetGPUVirtualAddress());
		command_list->SetGraphicsRootConstantBufferView(6, m_bone_trans_data.at(lod)->GetResource()->GetGPUVirtualAddress());

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

	void SkeletalMesh::UpdateBoneTransform() const
	{
		AnimationData temp_data;

		UINT trans_size = static_cast<UINT>(m_bone_data->bone_names.size());
		for (UINT i = 0; i < trans_size; ++i)
		{
			/*auto name = &m_cache_skeleton.at(i)->GetBoneName();
			if (name->compare("Bip001") == false)
			{
				auto temp = &m_cache_skeleton.at(i)->GetWorld();
				int j = 0;
			}*/
			temp_data.mat128.at(i) = m_cache_skeleton.at(i)->GetWorld();
		}
		m_bone_trans_data.at(0)->CopyData(0, AnimationData{ temp_data.mat128 });
		
	}


	
	
}