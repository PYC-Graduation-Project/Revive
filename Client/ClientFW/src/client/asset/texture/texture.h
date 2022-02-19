#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	enum class eTextureType
	{
		kExternal, kRedner
	};

	class Texture
	{
	public:
		Texture(eTextureType type);
		virtual ~Texture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) { return true; }

	protected:
		eTextureType m_texture_type;
		UINT m_texture_resource_index = 0;

		ComPtr<ID3D12Resource> m_texture_resource;
		
	public:
		eTextureType GetTextureType() const { return m_texture_type; }
		UINT GetResourceIndex() const { return m_texture_resource_index; }
		void SetResourceIndex(UINT index) { m_texture_resource_index = index; }
		ID3D12Resource* GetResource() const { return m_texture_resource.Get(); }
	};

	class ExternalTexture final : public Asset, public Texture
	{
	public:
		ExternalTexture();
		virtual ~ExternalTexture() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;

	private:
		ComPtr<ID3D12Resource> m_upload_heap;
	};

	class RenderTexture : public Texture
	{
	public:
		RenderTexture();
		virtual ~RenderTexture() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, 
			const std::vector<DXGI_FORMAT>& gbuffer_rtv_formats, const IVec2& size);
		virtual bool Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void GBufferPreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void GBufferPostDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		UINT m_num_of_gbuffer_texture;
		std::vector<ComPtr<ID3D12Resource>> m_gbuffer_textures;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_gbuffer_rtv_cpu_handles;
		D3D12_CPU_DESCRIPTOR_HANDLE m_rtv_cpu_handle;

		ComPtr<ID3D12Resource> m_gbuffer_dsv_texture;
		ComPtr<ID3D12Resource> m_dsv_texture;
		ComPtr<ID3D12DescriptorHeap> m_dsv_descriptor_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_gbuffer_dsv_cpu_handle;
		D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_cpu_handle;
		
	public:
		UINT GetNumOfGBufferTexture() const { return m_num_of_gbuffer_texture; }
		ID3D12Resource* GetGBufferTexture(UINT index);
		ID3D12Resource* GetDSVTexture() { return m_dsv_texture.Get(); }
	};
}


