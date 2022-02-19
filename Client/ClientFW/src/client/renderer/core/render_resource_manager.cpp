#include "stdafx.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/material/material.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderResourceManager* RenderResourceManager::s_render_resource_manager = nullptr;

	RenderResourceManager::RenderResourceManager()
	{
		s_render_resource_manager = this;
		m_material_data = CreateUPtr<UploadBuffer<RSMaterialData>>();
	}

	RenderResourceManager::~RenderResourceManager()
	{
	}

	bool RenderResourceManager::Initialize(ID3D12Device* device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC texture_heap_desc;
		texture_heap_desc.NumDescriptors = 8192;
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
		UpdateTextureResource(device, command_list);

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
			switch (texture->GetTextureType())
			{
			case eTextureType::kExternal:
			{
				const auto& external_texture = std::static_pointer_cast<ExternalTexture>(texture);
				if (m_initialized_assets.find(external_texture->GetPath()) == m_initialized_assets.cend())
				{
					m_initialized_assets.insert(external_texture->GetPath());
					m_ready_external_textures.push_back(external_texture);
				}
				break;
			}
			case eTextureType::kRedner:
			{
				m_ready_render_textures.push_back(std::static_pointer_cast<RenderTexture>(texture));
				break;
			}
			default:
				break;
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

	void RenderResourceManager::UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart());
		//D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = heap->GetGPUDescriptorHandleForHeapStart();

		cpu_handle.Offset(m_num_of_texture_data, D3DUtil::s_cbvsrvuav_descirptor_increment_size);

		for (const auto& texture : m_ready_external_textures)
		{
			texture->Initialize(device, command_list);
			
			device->CreateShaderResourceView(texture->GetResource(), 
				&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);

			texture->SetResourceIndex(m_num_of_texture_data++);
			cpu_handle.Offset(1, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
			//gpu_handle.ptr += num_of_data * D3DUtil::s_cbvsrvuav_descirptor_increment_size;

			LOG_INFO(texture->GetPath());
		}

		m_ready_external_textures.clear();

		for (const auto& texture : m_ready_render_textures)
		{
			//Level이 재생성 할 때 마다 새로운 Camera가 생기고,
			//그에 따른 ResourceView도 생기니까 낭비가 있다.
			//그래서 방법은 여러가지인데
			//Level마다 초기화 (Asset도 다시 Load해야함)
			//이 데이터를 넣는 구간을 따로 지정해서 그 부분만 초기화 한다.
			//일단은 RTT를 하는 것이 목표이기 때문에 현재로서는 낭비를 하고 들어가겠다.
			for (UINT i = 0; i < texture->GetNumOfGBufferTexture(); ++i)
			{
				device->CreateShaderResourceView(texture->GetGBufferTexture(i),
					&TextureCreator::GetShaderResourceViewDesc(texture->GetGBufferTexture(i)), cpu_handle);
				texture->SetGBufferResourceIndex(i, m_num_of_texture_data++);
				cpu_handle.Offset(1, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
			}

			device->CreateShaderResourceView(texture->GetResource(),
				&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);
			texture->SetResourceIndex(m_num_of_texture_data++);
			cpu_handle.Offset(1, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
			
			device->CreateShaderResourceView(texture->GetDSVTexture(),
				&TextureCreator::GetShaderResourceViewDescForDSV(texture->GetDSVTexture()), cpu_handle);
			texture->SetDSVResourceIndex(m_num_of_texture_data++);
			cpu_handle.Offset(1, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
		}

		m_ready_render_textures.clear();
	}
}
