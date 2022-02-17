#include "stdafx.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"

namespace client_fw
{
	Texture::Texture()
	{
	}
	Texture::~Texture()
	{
	}

	bool Texture::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_is_external_file)
		{
			m_texture_resource = TextureCreator::LoadTextureFromFile(device,
				command_list, GetPath(), GetExtension(), m_upload_heap);

			if (m_texture_resource != nullptr)
			{
				std::wstring w_name(GetName().begin(), GetName().end());
				m_texture_resource->SetName(w_name.c_str());
			}
		}

		return (m_texture_resource != nullptr);
	}
	void Texture::Shutdown()
	{
	}
}
