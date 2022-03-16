#include "stdafx.h"
#include "client/renderer/renderitem/mesh_render_item.h"
#include "client/renderer/core/render.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	MeshRenderItem::MeshRenderItem()
	{
		m_instance_data = CreateUPtr<UploadBuffer<RSInstanceData>>(false);
	}

	MeshRenderItem::~MeshRenderItem()
	{
	}

	void MeshRenderItem::Initialize(ID3D12Device* device)
	{
	}

	void MeshRenderItem::Shutdown()
	{
		m_instance_data->Shutdown();
	}

	void MeshRenderItem::Update(ID3D12Device* device)
	{
		if (m_mesh_data.empty() == false)
		{
			if (m_is_need_resource_create)
			{
				CreateResources(device);
				m_is_need_resource_create = false;
			}
			UpdateResources();
		}
	}

	void MeshRenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		UINT start_index = 0;
		for (const auto& mesh_data : m_mesh_data)
		{
			const auto& mesh = mesh_data->mesh;

			UpdateResourcesBeforeDraw(mesh_data, start_index);

			if (mesh_data->draw_start_index >= 0)
			{
				mesh->PreDraw(command_list);

				for (UINT lod = 0; lod < mesh->GetLODCount(); ++lod)
				{
					if (mesh->GetLODMeshCount(lod) > 0)
					{
						command_list->SetGraphicsRootShaderResourceView(1, m_instance_data->GetResource()->GetGPUVirtualAddress() +
							(mesh_data->draw_start_index + mesh_data->index_of_lod_instance_data[lod]) * m_instance_data->GetByteSize());

						mesh->Draw(command_list, lod);
					}
				}
			}
			mesh_data->mesh->PostDraw(command_list);
		}
	}

	void MeshRenderItem::CreateResources(ID3D12Device* device)
	{
		m_instance_data->CreateResource(device, m_num_of_instance_data);
	}

	void MeshRenderItem::UpdateResources()
	{
		for (const auto& mesh_data : m_mesh_data)
		{
			for (auto& mesh_comp_data : mesh_data->mesh_comp_data)
			{
				const auto& mesh_comp = mesh_comp_data.mesh_comp;

				if (mesh_comp->IsUpdatedWorldMatrix() || mesh_comp_data.is_need_update)
				{
					Mat4 world_matrix = mesh_comp->GetWorldMatrix();
					mesh_comp_data.world_transpose = mat4::Transpose(world_matrix);
					mesh_comp_data.world_inverse = mat4::InverseVec(world_matrix);
					mesh_comp_data.is_need_update = false;
				}
			}
		}

	}

	void MeshRenderItem::UpdateResourcesBeforeDraw(const SPtr<MeshData>& mesh_data, UINT& start_index)
	{
		mesh_data->index_of_lod_instance_data.resize(mesh_data->mesh->GetLODCount(), 0);
		mesh_data->index_of_lod_instance_data[0] = mesh_data->mesh->GetLODMeshCount(0);
		for (size_t lod = 1; lod < mesh_data->mesh->GetLODCount(); ++lod)
		{
			mesh_data->index_of_lod_instance_data[lod] =
				mesh_data->index_of_lod_instance_data[lod - 1] + mesh_data->mesh->GetLODMeshCount(static_cast<UINT>(lod));
		}

		UINT index = 0;

		for (auto& mesh_comp_data : mesh_data->mesh_comp_data)
		{
			const auto& mesh_comp = mesh_comp_data.mesh_comp;

			if (mesh_comp->IsVisible())
			{
				UINT lod = mesh_comp->GetLevelOfDetail();

				m_instance_data->CopyData(start_index + --mesh_data->index_of_lod_instance_data.at(lod),
					RSInstanceData{ mesh_comp_data.world_transpose, mesh_comp_data.world_inverse });

				mesh_comp->SetVisiblity(false);

				++index;
			}
		}

		if (index == 0)
			mesh_data->draw_start_index = -1;
		else
			mesh_data->draw_start_index = start_index;

		start_index += index;
	}

	void MeshRenderItem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		std::string path = mesh_comp->GetMesh()->GetPath();

		if (m_mesh_data_map.find(path) != m_mesh_data_map.cend())
		{
			MeshComponentData data;
			data.mesh_comp = mesh_comp;
			data.mesh_comp->SetRenderItemIndex(static_cast<UINT>(m_mesh_data_map[path]->mesh_comp_data.size()));
			data.is_need_update = true;

			m_mesh_data_map[path]->mesh_comp_data.emplace_back(std::move(data));
		}
		else
		{
			SPtr<MeshData> mesh_data = CreateSPtr<MeshData>();
			mesh_data->mesh = mesh_comp->GetMesh();
			mesh_data->index_of_lod_instance_data.resize(mesh_data->mesh->GetLODCount(), 0);
			MeshComponentData data;
			data.mesh_comp = mesh_comp;
			data.mesh_comp->SetRenderItemIndex(0);
			data.is_need_update = true;
			mesh_data->mesh_comp_data.emplace_back(std::move(data));

			m_mesh_data.push_back(mesh_data);
			m_mesh_data_map.insert({ path, mesh_data });
		}

		++m_real_num_of_instance_data;

		if (m_num_of_instance_data == 0)
		{
			m_num_of_instance_data = 1;
			m_is_need_resource_create = true;
		}
		else if (m_num_of_instance_data < m_real_num_of_instance_data)
		{
			m_num_of_instance_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_instance_data) * 1.5f));
			m_is_need_resource_create = true;
		}

	}

	void MeshRenderItem::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		std::string path = mesh_comp->GetMesh()->GetPath();

		if (m_mesh_data_map.find(path) != m_mesh_data_map.cend())
		{
			UINT index = mesh_comp->GetRenderItemIndex();

			auto& mesh_data = m_mesh_data_map[path];

			std::swap(*(mesh_data->mesh_comp_data.begin() + index), *(mesh_data->mesh_comp_data.end() - 1));
			mesh_data->mesh_comp_data[index].mesh_comp->SetRenderItemIndex(index);
			mesh_data->mesh_comp_data[index].is_need_update = true;

			mesh_data->mesh_comp_data.pop_back();

		/*	if (mesh_data->mesh_comp_data.empty())
				mesh_comp->GetMesh()->ResetLOD();*/

			--m_real_num_of_instance_data;

			UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_instance_data * 0.66f)));
			if (size >= m_real_num_of_instance_data)
			{
				if (m_real_num_of_instance_data == 0)
				{
					m_num_of_instance_data = 0;
				}
				else
				{
					m_num_of_instance_data = size;
					m_is_need_resource_create = true;
				}
			}
		}
	}
}

