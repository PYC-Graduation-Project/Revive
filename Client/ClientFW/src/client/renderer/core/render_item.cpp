#include "stdafx.h"
#include "client/renderer/core/render_item.h"
#include "client/renderer/core/render.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/object/animation/animation_controller.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/mesh/skeletal_mesh_component.h"
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	MeshRenderItem::MeshRenderItem(const SPtr<Mesh>& mesh)
		: m_mesh(mesh)
	{
		m_index_of_lod_instance_data.resize(m_mesh->GetLODCount(), 0);
		m_instance_data = CreateUPtr<UploadBuffer<RSInstanceData>>(false);
		m_bone_trans_data = CreateUPtr<UploadBuffer<RSSkletalData>>(false);
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
		m_bone_trans_data->Shutdown();
	}

	void MeshRenderItem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
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

	void MeshRenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_mesh != nullptr && m_mesh_comp_data.empty() == false)
		{
			UpdateResourcesBeforeDraw();

			m_mesh->PreDraw(command_list);

			if (m_animation_controllers.empty() == false)
				command_list->SetGraphicsRootShaderResourceView(5, m_bone_trans_data->GetResource()->GetGPUVirtualAddress());

			for (UINT lod = 0; lod < m_mesh->GetLODCount(); ++lod)
			{
				if (m_mesh->GetLODMeshCount(lod) > 0)
				{
					command_list->SetGraphicsRootShaderResourceView(1, m_instance_data->GetResource()->GetGPUVirtualAddress() +
						m_index_of_lod_instance_data[lod] * m_instance_data->GetByteSize());
					m_mesh->Draw(command_list, lod);
				}

			}

			m_mesh->PostDraw(command_list);
		}
	}

	void MeshRenderItem::CreateResources(ID3D12Device* device)
	{
		//LOG_INFO(m_num_of_instance_data);
		m_instance_data->CreateResource(device, m_num_of_instance_data);
		m_bone_trans_data->CreateResource(device, m_num_of_instance_data);
	}

	void MeshRenderItem::UpdateResources()
	{
		for (auto& mesh_data : m_mesh_comp_data)
		{
			const auto& mesh_comp = mesh_data.mesh_comp;

			if (mesh_comp->IsUpdatedWorldMatrix() || mesh_data.is_need_update)
			{
				Mat4 world_matrix = mesh_comp->GetWorldMatrix();
				mesh_data.world_transpose = mat4::Transpose(world_matrix);
				mesh_data.world_inverse = mat4::InverseVec(world_matrix);
				mesh_data.is_need_update = false;
			}
		}
	}

	void MeshRenderItem::UpdateResourcesBeforeDraw()
	{
		m_index_of_lod_instance_data.resize(m_mesh->GetLODCount(), 0);
		m_index_of_lod_instance_data[0] = m_mesh->GetLODMeshCount(0);
		for (size_t lod = 1; lod < m_mesh->GetLODCount(); ++lod)
		{
			m_index_of_lod_instance_data[lod] =
				m_index_of_lod_instance_data[lod - 1] + m_mesh->GetLODMeshCount(static_cast<UINT>(lod));
		}

		UINT index = m_mesh->GetLODMeshCount(0);

		for (auto& animation_controller : m_animation_controllers)
		{
			//if (animation_controller->IsVisible())
			//{
			if (index <= 0) break;
				m_bone_trans_data->CopyData(--index,
					RSSkletalData{ animation_controller->GetBoneTransformData() });
			//}
		}

		for (auto& mesh_data : m_mesh_comp_data)
		{
			const auto& mesh_comp = mesh_data.mesh_comp;

			if (mesh_comp->IsVisible())
			{
				UINT lod = mesh_comp->GetLevelOfDetail();

				m_instance_data->CopyData(--m_index_of_lod_instance_data.at(lod),
					RSInstanceData{ mesh_data.world_transpose, mesh_data.world_inverse });

				mesh_comp->SetVisiblity(false);
			}
		}
	}

	void MeshRenderItem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
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

	void MeshRenderItem::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		UINT index = mesh_comp->GetInstanceIndex();

		std::swap(*(m_mesh_comp_data.begin() + index), *(m_mesh_comp_data.end() - 1));
		m_mesh_comp_data[index].mesh_comp->SetInstanceIndex(index);
		m_mesh_comp_data[index].is_need_update = true;

		m_mesh_comp_data.pop_back();


		UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_instance_data * 0.66f)));
		if (size >= m_mesh_comp_data.size())
		{
			if (m_mesh_comp_data.empty())
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

	void MeshRenderItem::RegisterSkeletalMeshComponent(const SPtr<SkeletalMeshComponent>& skeletal_mesh_component )
	{
		m_animation_controllers.emplace_back(skeletal_mesh_component->GetAnimationController());
	}
	void MeshRenderItem::UnregisterSkeletalMeshComponent(const SPtr<SkeletalMeshComponent>& skeletal_mesh_component)
	{
		//메시 데이터를 기준으로 삭제한다.
		UINT index = skeletal_mesh_component->GetInstanceIndex();
		std::swap(*(m_animation_controllers.begin() + index), *(m_animation_controllers.end() - 1));

		m_animation_controllers.pop_back();
	}
}
