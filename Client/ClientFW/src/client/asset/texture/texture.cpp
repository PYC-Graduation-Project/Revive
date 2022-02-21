#include "stdafx.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"
#include "client/asset/core/asset_store.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	Texture::Texture(eTextureType type)
		: m_texture_type(type)
	{
	}

	Texture::~Texture()
	{
	}

	ExternalTexture::ExternalTexture()
		: Asset(), Texture(eTextureType::kExternal)
	{
	}

	bool ExternalTexture::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_texture_resource = TextureCreator::LoadTextureFromFile(device,
			command_list, GetPath(), GetExtension(), m_upload_heap);

		if (m_texture_resource != nullptr)
		{
			std::wstring w_name(GetName().begin(), GetName().end());
			m_texture_resource->SetName(w_name.c_str());
		}

		return (m_texture_resource != nullptr);
	}

	RenderTexture::RenderTexture(const IVec2& size)
		: Texture(eTextureType::kRedner)
		, m_texture_size(size)
	{
	}

	RenderTexture::~RenderTexture()
	{
	}

	bool RenderTexture::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
		const std::vector<DXGI_FORMAT>& gbuffer_rtv_formats)
	{
		m_num_of_gbuffer_texture = static_cast<UINT>(gbuffer_rtv_formats.size());


		D3D12_CLEAR_VALUE rtv_clear_value{ DXGI_FORMAT_R8G8B8A8_UNORM, {0.0f, 0.0f, 0.0f, 1.0f} };
		for (UINT i = 0; i < m_num_of_gbuffer_texture; ++i)
		{
			rtv_clear_value.Format = gbuffer_rtv_formats[i];
			m_gbuffer_textures.emplace_back(TextureCreator::Create2DTexture(device, gbuffer_rtv_formats[i], m_texture_size, 0,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &rtv_clear_value));
		}

		rtv_clear_value.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_texture_resource = TextureCreator::Create2DTexture(device, DXGI_FORMAT_R8G8B8A8_UNORM, m_texture_size, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &rtv_clear_value);

		D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc;
		rtv_heap_desc.NumDescriptors = m_num_of_gbuffer_texture + 1;
		rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtv_heap_desc.NodeMask = 0;
		if (FAILED(device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_rtv_descriptor_heap))))
		{
			LOG_ERROR("Could not create RTV descriptor heap");
			return false;
		}

		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc;
		rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Texture2D.MipSlice = 0;
		rtv_desc.Texture2D.PlaneSlice = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_heap_handle(m_rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < m_num_of_gbuffer_texture; ++i)
		{
			rtv_desc.Format = gbuffer_rtv_formats[i];
			device->CreateRenderTargetView(m_gbuffer_textures[i].Get(), &rtv_desc, rtv_heap_handle);
			m_gbuffer_rtv_cpu_handles.push_back(rtv_heap_handle);
			m_gbuffer_texture_resource_indices.push_back(0);
			rtv_heap_handle.Offset(1, D3DUtil::s_rtv_descirptor_increment_size);
		}
		rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		device->CreateRenderTargetView(m_texture_resource.Get(), &rtv_desc, rtv_heap_handle);
		m_rtv_cpu_handle = rtv_heap_handle;

		D3D12_CLEAR_VALUE dsv_clear_value{ DXGI_FORMAT_D24_UNORM_S8_UINT, {1.0f, 0} };

		m_dsv_texture = TextureCreator::Create2DTexture(device, DXGI_FORMAT_R24G8_TYPELESS, m_texture_size, 1,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE, &dsv_clear_value);

		D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc;
		dsv_heap_desc.NumDescriptors = 1;
		dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsv_heap_desc.NodeMask = 0;
		if (FAILED(device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&m_dsv_descriptor_heap))))
		{
			LOG_ERROR("Could not create DSV descriptor heap");
			return false;
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc;
		dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Flags = D3D12_DSV_FLAG_NONE;
		dsv_desc.Texture2D.MipSlice = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE dsv_heap_handle(m_dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart());
		device->CreateDepthStencilView(m_dsv_texture.Get(), &dsv_desc, dsv_heap_handle);
		m_dsv_cpu_handle = dsv_heap_handle;

		return true;
	}

	bool RenderTexture::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		return true;
	}

	void RenderTexture::GBufferPreDraw(ID3D12GraphicsCommandList* command_list)
	{
		for (UINT i = 0; i < m_num_of_gbuffer_texture; ++i)
		{
			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_gbuffer_textures[i].Get(),
				D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET));
			command_list->ClearRenderTargetView(m_gbuffer_rtv_cpu_handles[i], Colors::Black, 0, nullptr);
		}

		command_list->ClearDepthStencilView(m_dsv_cpu_handle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		command_list->OMSetRenderTargets(m_num_of_gbuffer_texture, m_gbuffer_rtv_cpu_handles.data(), FALSE, &m_dsv_cpu_handle);
	}

	void RenderTexture::GBufferPostDraw(ID3D12GraphicsCommandList* command_list)
	{
		for (UINT i = 0; i < m_num_of_gbuffer_texture; ++i)
		{
			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_gbuffer_textures[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ));
		}
	}

	void RenderTexture::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			m_texture_resource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET));

		FLOAT clear_value[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		command_list->ClearRenderTargetView(m_rtv_cpu_handle, clear_value, 0, nullptr);
		command_list->OMSetRenderTargets(1, &m_rtv_cpu_handle, true, nullptr);
	}

	void RenderTexture::PostDraw(ID3D12GraphicsCommandList* command_list)
	{
		command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			m_texture_resource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ));
	}

	ID3D12Resource* RenderTexture::GetGBufferTexture(UINT buffer_index) const
	{
		if (buffer_index < m_num_of_gbuffer_texture)
			return m_gbuffer_textures[buffer_index].Get();
		else
		{
			LOG_WARN("Out of range of texture GBuffer Resource");
			return nullptr;
		}
	}

	UINT RenderTexture::GetGBufferResourceIndex(UINT buffer_index) const
	{
		if (buffer_index < m_num_of_gbuffer_texture)
			return m_gbuffer_texture_resource_indices[buffer_index];
		else
		{
			LOG_WARN("Out of range of texture GBuffer Index");
			return 0;
		}
	}

	void RenderTexture::SetGBufferResourceIndex(UINT buffer_index, UINT resource_index)
	{
		if (buffer_index < m_num_of_gbuffer_texture)
			m_gbuffer_texture_resource_indices[buffer_index] = resource_index;
		else
			LOG_WARN("Out of range of texture GBuffer Index");
	}



}
