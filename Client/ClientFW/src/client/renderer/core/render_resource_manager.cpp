#include "stdafx.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/material/material.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"
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

	bool RenderResourceManager::Initialize(ID3D12Device* device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC texture_heap_desc;
		texture_heap_desc.NumDescriptors = 1024;
		texture_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		texture_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		texture_heap_desc.NodeMask = 0;

		if (FAILED(device->CreateDescriptorHeap(&texture_heap_desc, IID_PPV_ARGS(m_texture_desciptor_heap.GetAddressOf()))))
		{
			LOG_ERROR("Could not create texture descriptor heap");
			return false;
		}
		m_texture_desciptor_heap->SetName(L"Texture descriptor");
		m_num_of_texture_data = 0;

		return true;
	}

	void RenderResourceManager::Shutdown()
	{
		m_material_data->Shutdown();
	}

	void RenderResourceManager::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_ready_textures.empty() == false)
		{
			CreateTextureResource(device);
			UpdateTextureResource(device, command_list);
			m_ready_textures.clear();
		}

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

	void RenderResourceManager::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		ID3D12DescriptorHeap* descriptor_heaps[] = { m_texture_desciptor_heap.Get() };
		command_list->SetDescriptorHeaps(_countof(descriptor_heaps), descriptor_heaps);

		if (m_material_data->GetResource() != nullptr)
			command_list->SetGraphicsRootShaderResourceView(3, m_material_data->GetResource()->GetGPUVirtualAddress());

		command_list->SetGraphicsRootDescriptorTable(4, m_texture_desciptor_heap->GetGPUDescriptorHandleForHeapStart());
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

	void RenderResourceManager::RegisterTexture(const SPtr<Texture>& texture)
	{
		if (texture != nullptr)
		{
			if (m_initialized_assets.find(texture->GetPath()) == m_initialized_assets.cend())
			{
				m_initialized_assets.insert(texture->GetPath());
				m_ready_textures.push_back(texture);
			}
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

				RegisterTexture(material->GetDiffuseTexture());
			}
		}
	}

	void RenderResourceManager::CreateMaterialResource(ID3D12Device* device)
	{
		m_material_data->CreateAndCopyResource(device, m_num_of_material_data +
			static_cast<UINT>(m_ready_materials.size()));
	}

	void RenderResourceManager::UpdateMaterialResource()
	{
		for (const auto& material : m_ready_materials)
		{
			material->SetResourceIndex(m_num_of_material_data);
			INT diffuse_index = -1;
			if(material->GetDiffuseTexture() != nullptr)
				diffuse_index = material->GetDiffuseTexture()->GetResourceIndex();
			RSMaterialData data{ material->GetBaseColor(), diffuse_index };

			m_material_data->CopyData(m_num_of_material_data++, data);
		}
	}

	void RenderResourceManager::CreateTextureResource(ID3D12Device* device)
	{
	}

	void RenderResourceManager::UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart());
		//D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = heap->GetGPUDescriptorHandleForHeapStart();

		cpu_handle.Offset(m_num_of_texture_data, D3DUtil::s_cbvsrvuav_descirptor_increment_size);

		for (const auto& texture : m_ready_textures)
		{
			texture->Initialize(device, command_list);
			
			device->CreateShaderResourceView(texture->GetResource(), 
				&TextureCreator::GetShaderResourceViewDesc(texture), cpu_handle);

			texture->SetResourceIndex(m_num_of_texture_data++);
			cpu_handle.Offset(1, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
			//gpu_handle.ptr += num_of_data * D3DUtil::s_cbvsrvuav_descirptor_increment_size;

			LOG_INFO(texture->GetPath());
		}
	}
}
