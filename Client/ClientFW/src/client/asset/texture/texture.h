#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	enum class eTextureType
	{
		kDiffuse,
	};

	class Texture :	public Asset
	{
	public:
		Texture();
		virtual ~Texture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void Shutdown() override;

	private:
		bool m_is_external_file = true;
		eTextureType m_texture_type;
		UINT m_texture_resource_index = 0;

		ComPtr<ID3D12Resource> m_texture_resource;
		ComPtr<ID3D12Resource> m_upload_heap;

	public:
		void SetIsExternal(bool value) { m_is_external_file = value; }
		eTextureType GetTextureType() const { return m_texture_type; }
		void SetTextureType(eTextureType type) { m_texture_type = type; }
		UINT GetResourceIndex() const { return m_texture_resource_index; }
		void SetResourceIndex(UINT index) { m_texture_resource_index = index; }
		ID3D12Resource* GetResource() const { return m_texture_resource.Get(); }
	};
}


