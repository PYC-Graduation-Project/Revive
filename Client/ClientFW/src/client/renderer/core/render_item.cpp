#include "stdafx.h"
#include "client/renderer/core/render_item.h"
#include "client/renderer/core/render.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/material.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderItem::RenderItem(const SPtr<Mesh>& mesh)
		: m_mesh(mesh)
	{
		for (UINT lod = 0; lod < m_mesh->GetLODCount(); ++lod)
		{
			m_lod_material_indexs.push_back(m_material_count);
			m_lod_material_counts.push_back(static_cast<UINT>(m_mesh->GetMaterials(lod).size()));
			m_material_count += m_lod_material_counts[lod];
		}
		m_index_of_lod_instance_data.resize(m_mesh->GetLODCount(), 0);
		m_instance_data = CreateUPtr<UploadBuffer<RSInstanceData>>(false);
		m_material_index_data = CreateUPtr<UploadBuffer<RSMaterialIndexData>>(true);
	}

	RenderItem::~RenderItem()
	{
	}

	void RenderItem::Initialize(ID3D12Device* device)
	{
		m_material_index_data->CreateResource(device, m_material_count);
	}

	void RenderItem::Shutdown()
	{
		m_instance_data->Shutdown();
		m_material_index_data->Shutdown();
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
		
			//이 코드는 사실 Material data들의 저장 index가 바뀌지 않으면 Update를
			//한번만 해주면 된다. 임시로 코드는 이렇게 작성하였다.
			if (m_is_updated_material_index_data == false)
			{
				UpdateMaterialIndexResource();
			}
		}
	}

	void RenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		UpdateResourcesBeforeDraw();

		if (m_mesh != nullptr && m_mesh_comp_data.empty() == false)
		{
			
			m_mesh->PreDraw(command_list);

			auto material_index_resource = m_material_index_data->GetResource();
			UINT material_index_resource_byte_size = m_material_index_data->GetByteSize();

			for (UINT lod = 0; lod < m_mesh->GetLODCount(); ++lod)
			{
				if (m_mesh->GetLODMeshCount(lod) > 0)
				{
					command_list->SetGraphicsRootShaderResourceView(1, m_instance_data->GetResource()->GetGPUVirtualAddress() +
						m_index_of_lod_instance_data[lod] * m_instance_data->GetByteSize());

					m_mesh->PreDraw(command_list, lod);

					for (UINT i = 0; i < m_lod_material_counts[lod]; ++i)
					{
						command_list->SetGraphicsRootConstantBufferView(0, material_index_resource->GetGPUVirtualAddress() +
							(i + m_lod_material_indexs[lod]) * material_index_resource_byte_size);
						m_mesh->Draw(command_list, lod, i);
					}
				}
				
			}

			m_mesh->PostDraw(command_list);
		}
	}

	void RenderItem::CreateResources(ID3D12Device* device)
	{
		//LOG_INFO(m_num_of_instance_data);
		m_instance_data->CreateResource(device, m_num_of_instance_data);
	}

	void RenderItem::UpdateResources()
	{
		for (auto& mesh_data : m_mesh_comp_data)
		{
			const auto& mesh_comp = mesh_data.mesh_comp;
			const auto& owner = mesh_comp->GetOwner().lock();

			if (owner != nullptr && (owner->IsUpdatedWorldMatrix() || mesh_data.is_need_update))
			{
				Mat4 world_matrix = owner->GetWorldMatrix();
				mesh_data.world_transpose = mat4::Transpose(world_matrix);
				mesh_data.world_inverse = mat4::InverseVec(world_matrix);
				mesh_data.is_need_update = false;
			}
		}
	}

	void RenderItem::UpdateResourcesBeforeDraw()
	{
		m_index_of_lod_instance_data.resize(m_mesh->GetLODCount(), 0);
		m_index_of_lod_instance_data[0] = m_mesh->GetLODMeshCount(0);
		for (size_t lod = 1; lod < m_mesh->GetLODCount(); ++lod)
		{
			m_index_of_lod_instance_data[lod] =
				m_index_of_lod_instance_data[lod - 1] + m_mesh->GetLODMeshCount(static_cast<UINT>(lod));
		}

		for (auto& mesh_data : m_mesh_comp_data)
		{
			const auto& mesh_comp = mesh_data.mesh_comp;
			const auto& owner = mesh_comp->GetOwner().lock();

			if (owner != nullptr && mesh_comp->IsVisible())
			{
				UINT lod = mesh_comp->GetLevelOfDetail();

				m_instance_data->CopyData(--m_index_of_lod_instance_data.at(lod),
					RSInstanceData{ mesh_data.world_transpose, mesh_data.world_inverse });

				mesh_comp->SetVisiblity(false);
			}
		}
	}

	void RenderItem::UpdateMaterialIndexResource()
	{
		UINT index = 0;
		for (UINT lod = 0; lod < m_mesh->GetLODCount(); ++lod)
		{
			for (const auto& material : m_mesh->GetMaterials(lod))
			{
				m_material_index_data->CopyData(index, RSMaterialIndexData{ material->GetResourceIndex() });
				++index;
			}
		}
		m_is_updated_material_index_data = true;
	}

	void RenderItem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		const auto& owner = mesh_comp->GetOwner().lock();

		MeshComponentData data;
		data.mesh_comp = mesh_comp;
		data.mesh_comp->SetInstanceIndex(static_cast<UINT>(m_mesh_comp_data.size()));
		data.is_need_update = true;

		m_mesh_comp_data.emplace_back(std::move(data));

		if (m_num_of_instance_data == 0)
		{
			m_num_of_instance_data = 1;
			m_is_need_resource_create = true;
		}
		else if (m_num_of_instance_data < static_cast<UINT>(m_mesh_comp_data.size()))
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
		m_mesh_comp_data[index].is_need_update = true;

		m_mesh_comp_data.pop_back();


		UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_instance_data * 0.66f)));
		if (size >= m_mesh_comp_data.size())
		{
			if(m_mesh_comp_data.empty())
			{
				m_num_of_instance_data = 0;
				m_instance_data->Shutdown();
			}
			else
			{
				m_num_of_instance_data = size;
				m_is_need_resource_create = true;
			}
		}
	}
}

