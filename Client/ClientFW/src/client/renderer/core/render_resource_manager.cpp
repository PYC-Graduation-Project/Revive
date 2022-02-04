#include "stdafx.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/material.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderResourceManager::RenderResourceManager()
	{
		m_material_data = CreateUPtr<UploadBuffer<RSMaterialData>>();
	}

	RenderResourceManager::~RenderResourceManager()
	{
	}

	void RenderResourceManager::Shutdown()
	{
		m_material_data->Shutdown();
	}

	void RenderResourceManager::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_ready_materials.empty() == false)
		{
			CreateMaterialResource(device);
			UpdateMaterialResource();
			m_ready_materials.clear();
		}

		for (const auto& mesh : m_ready_meshes)
		{
			mesh->Initialize(device, command_list);
			LOG_INFO(mesh->GetPath());
		}
		m_ready_meshes.clear();
	}

	void RenderResourceManager::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_material_data->GetResource() != nullptr)
			command_list->SetGraphicsRootShaderResourceView(3, m_material_data->GetResource()->GetGPUVirtualAddress());
	}

	void RenderResourceManager::RegisterMesh(const SPtr<Mesh>& mesh)
	{
		if (m_initialized_assets.find(mesh->GetPath()) == m_initialized_assets.cend())
		{
			m_initialized_assets.insert(mesh->GetPath());
			m_ready_meshes.push_back(mesh);
			
			for (UINT i = 0; i < mesh->GetLODCount(); ++i)
				RegisterMaterials(mesh->GetMaterials(i));
		}
	}

	void RenderResourceManager::RegisterMaterials(const std::vector<SPtr<Material>>& materials)
	{
		for (const auto& material : materials)
		{
			if (m_initialized_assets.find(material->GetPath()) == m_initialized_assets.cend())
			{
				m_initialized_assets.insert(material->GetPath());
				m_ready_materials.push_back(material);
			}
		}
	}

	void RenderResourceManager::CreateMaterialResource(ID3D12Device* device)
	{
		m_material_data->CreateResource(device, m_num_of_material_data +
			static_cast<UINT>(m_ready_materials.size()));
	}

	void RenderResourceManager::UpdateMaterialResource()
	{
		for (const auto& material : m_ready_materials)
		{
			material->SetResourceIndex(m_num_of_material_data);
			RSMaterialData data{ material->GetBaseColor() };

			m_material_data->CopyData(m_num_of_material_data++, data);
		}
	}
}
