#pragma once

namespace client_fw
{
#define START_INDEX_EXTERNAL_TEXTURE		0
#define START_INDEX_RENDER_TEXTURE			8192
#define START_INDEX_RENDER_TEXT_TEXTURE		11264
#define START_INDEX_CUBE_MAP_TEXTURE		12288

	class Primitive;
	class Mesh;
	class Material;
	class Texture;
	class RenderTextTexture;
	class ExternalTexture;
	class ExternalCubeMapTexture;
	class RenderTexture;
	enum class eTextureType;

	class RenderResourceManager final
	{
	public:
		RenderResourceManager();
		~RenderResourceManager();

		RenderResourceManager(const RenderResourceManager&) = delete;
		RenderResourceManager& operator=(const RenderResourceManager&) = delete;

		bool Initialize(ID3D12Device* device);
		void Shutdown();
		void PreDraw(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list) const;

		void RegisterPrimitive(const SPtr<Primitive>& primitive);
		void RegisterMaterial(const SPtr<Material>& material);
		void RegisterTexture(const SPtr<Texture>& texture);

	private:
		void UpdatePrimiviteResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateMaterialResource(ID3D12Device* device);

		void UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateExternalTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateExternalCubeMapTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateRenderTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateRenderTextTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	private:
		static RenderResourceManager* s_render_resource_manager;

		std::vector<SPtr<Material>> m_materials;
		
		std::vector<SPtr<Primitive>> m_ready_primitives;
		std::vector<SPtr<ExternalTexture>> m_ready_external_textures;
		std::vector<SPtr<RenderTexture>> m_ready_render_textures;
		std::vector<SPtr<RenderTextTexture>> m_ready_render_text_textures;
		std::vector<SPtr<ExternalCubeMapTexture>> m_ready_external_cube_map_textures;

	private:
		UINT m_num_of_external_texture_data = START_INDEX_EXTERNAL_TEXTURE;
		UINT m_num_of_external_cube_map_texture_data = START_INDEX_CUBE_MAP_TEXTURE;
		UINT m_num_of_render_texture_data = START_INDEX_RENDER_TEXTURE;
		UINT m_num_of_render_text_texture_data = START_INDEX_RENDER_TEXT_TEXTURE;
		ComPtr<ID3D12DescriptorHeap> m_texture_desciptor_heap;

	public:
		static RenderResourceManager& GetRenderResourceManager() { return *s_render_resource_manager; }
	};
}

