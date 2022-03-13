#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	enum class eTextureType
	{
		kExternal, kRedner, kRenderUI
	};

	class Texture
	{
	public:
		Texture(eTextureType type);
		virtual ~Texture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) { return true; }
		virtual bool Initialize(ID3D12Device* device) { return true; }

	protected:
		eTextureType m_texture_type;
		INT m_texture_resource_index = -1;

		ComPtr<ID3D12Resource> m_texture_resource;
		
	public:
		eTextureType GetTextureType() const { return m_texture_type; }
		INT GetResourceIndex() const { return m_texture_resource_index; }
		void SetResourceIndex(INT index) { m_texture_resource_index = index; }
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
		RenderTexture(const IVec2& size);
		virtual ~RenderTexture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, 
			const std::vector<DXGI_FORMAT>& gbuffer_rtv_formats);
		virtual bool Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void GBufferPreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void GBufferPostDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		IVec2 m_texture_size;
		UINT m_num_of_gbuffer_texture;
		std::vector<ComPtr<ID3D12Resource>> m_gbuffer_textures;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_gbuffer_rtv_cpu_handles;
		std::vector<UINT> m_gbuffer_texture_resource_indices;
		D3D12_CPU_DESCRIPTOR_HANDLE m_rtv_cpu_handle;

		ComPtr<ID3D12Resource> m_dsv_texture;
		ComPtr<ID3D12DescriptorHeap> m_dsv_descriptor_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_cpu_handle;
		UINT m_dsv_texture_resource_index;
		
	public:
		const IVec2& GetTextureSize() const { return m_texture_size; }
		UINT GetNumOfGBufferTexture() const { return m_num_of_gbuffer_texture; }
		ID3D12Resource* GetGBufferTexture(UINT buffer_index) const;
		UINT GetGBufferResourceIndex(UINT buffer_index) const;
		void SetGBufferResourceIndex(UINT buffer_index, UINT resource_index);
		ID3D12Resource* GetDSVTexture() const { return m_dsv_texture.Get(); }
		UINT GetDSVResourceIndex() const { return m_dsv_texture_resource_index; }
		void SetDSVResourceIndex(UINT index) { m_dsv_texture_resource_index = index; }
	};

	class RenderTextTexture final : public Texture
	{
	public:
		RenderTextTexture(const IVec2& size);
		virtual ~RenderTextTexture() = default;

		virtual bool Initialize(ID3D12Device* device) override;
		bool Initialize2D(ID3D11On12Device* device, ID2D1DeviceContext2* device_context);

	private:
		IVec2 m_texture_size;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap;
		ComPtr<ID3D11Resource> m_wrapped_render_target;
		ComPtr<ID2D1Bitmap1> m_2d_render_target;

	public:
		ComPtr<ID3D11Resource> GetWrappedRenderTarget() const { return m_wrapped_render_target; }
		ID2D1Bitmap1* Get2DRenderTarget() const { return m_2d_render_target.Get(); }
	};
}


