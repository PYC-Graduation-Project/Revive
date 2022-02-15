#pragma once

namespace client_fw
{
	class Texture;

	class TextureLoader
	{
	public:
		TextureLoader() = default;

		TextureLoader(const TextureLoader&) = delete;
		TextureLoader& operator=(const TextureLoader&) = delete;

		virtual SPtr<Texture> LoadTexture(const std::string& path, const std::string& extension) const;
	};

	class TextureCreator
	{
	public:
		static ComPtr<ID3D12Resource> LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const std::string& path, const std::string& extension, ComPtr<ID3D12Resource>& texture_upload_heap);

	protected:
		static ComPtr<ID3D12Resource> LoadTextureFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const std::wstring& path, ComPtr<ID3D12Resource>& texture_upload_heap);

	public:
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(const SPtr<Texture>& texture);
	};
}


