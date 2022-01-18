#include "stdafx.h"
#include "client/renderer/core/render_item.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/material.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderItem::RenderItem(const SPtr<Mesh>& mesh, UINT material_count)
		: m_mesh(mesh), m_material_count(material_count)
	{
		m_material_count = static_cast<UINT>(m_mesh->GetMaterials().size());
		m_instance_data = CreateUPtr<UploadBuffer<RSInstanceData>>(false);
	}

	RenderItem::~RenderItem()
	{
	}

	void RenderItem::Shutdown()
	{
		m_instance_data->Shutdown();
	}

	void RenderItem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_mesh_comp_data.empty() == false)
		{
			if (m_is_need_resource_create)
			{
				CreateResources(device);
				m_is_need_resource_create = false;
			}
			UpdateResources();
		}
	}

	void RenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_mesh != nullptr && m_mesh_comp_data.empty() == false)
		{
			command_list->SetGraphicsRootShaderResourceView(0, m_instance_data->GetResource()->GetGPUVirtualAddress());
			m_mesh->PreDraw(command_list);
			for (UINT i = 0; i < m_material_count; ++i)
			{
				m_mesh->Draw(command_list, static_cast<UINT>(m_mesh_comp_data.size()), i);
			}
		}
	}

	void RenderItem::CreateResources(ID3D12Device* device)
	{
		m_instance_data->CreateResource(device, m_num_of_instance_data);
	}

	void RenderItem::UpdateResources()
	{
		UINT index = 0;

		//BYTE* mapped_data = m_instance_data->GetMappedData();
		//UINT byte_size = m_instance_data->GetByteSize();

		for (auto& mesh_data : m_mesh_comp_data)
		{
			const auto& mesh_comp = mesh_data.mesh_comp;
			const auto& owner = mesh_comp->GetOwner().lock();

			if (owner != nullptr && (owner->IsUpdatedWorldMatrix() || mesh_data.m_is_need_update))
			{
				Mat4 world_matrix = owner->GetWorldMatrix();
				Mat4 world_inverse_transpose = mat4::InverseVec(world_matrix);
				world_matrix.Transpose();

				RSInstanceData data{ world_matrix, world_inverse_transpose };
				
				//memcpy(&mapped_data[index * byte_size], &data, sizeof(InstanceData));

				m_instance_data->CopyData(index, data);

				mesh_data.m_is_need_update = false;
			}
			++index;
		}
	}

	void RenderItem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		const auto& owner = mesh_comp->GetOwner().lock();

		MeshComponentData data;
		data.mesh_comp = mesh_comp;
		data.mesh_comp->SetInstanceIndex(static_cast<UINT>(m_mesh_comp_data.size()));
		data.m_is_need_update = true;

		m_mesh_comp_data.emplace_back(std::move(data));

		if (m_num_of_instance_data < static_cast<UINT>(m_mesh_comp_data.size()))
		{
			m_num_of_instance_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_instance_data) * 1.5f));
			m_is_need_resource_create = true;
		}
	}

	void RenderItem::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		UINT index = mesh_comp->GetInstanceIndex();

		std::swap(*(m_mesh_comp_data.begin() + index), *(m_mesh_comp_data.end() - 1));
		m_mesh_comp_data[index].mesh_comp->SetInstanceIndex(index);
		m_mesh_comp_data[index].m_is_need_update = true;

		m_mesh_comp_data.pop_back();

		UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_instance_data * 0.66f)));
		if (size >= m_mesh_comp_data.size())
		{
			m_num_of_instance_data = size;
			m_is_need_resource_create = true;
		}
	}
}

