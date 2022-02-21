#pragma once

namespace client_fw
{
	class Mesh;
	class Material;
	class Texture;
	class ExternalTexture;
	class RenderTexture;
	enum class eTextureType;
	template<class T> class UploadBuffer;

	struct RSMaterialData
	{
		Vec4 base_color;
		INT diffuse_texture_index;
	};

	class RenderResourceManager final
	{
	public:
		RenderResourceManager();
		~RenderResourceManager();

		RenderResourceManager(const RenderResourceManager&) = delete;
		RenderResourceManager& operator=(const RenderResourceManager&) = delete;

		bool Initialize(ID3D12Device* device);
		void Shutdown();
		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list) const;

		void RegisterMesh(const SPtr<Mesh>& mesh);
		void RegisterMaterial(const SPtr<Material>& material);
		void RegisterTexture(const SPtr<Texture>& texture);

	private:
		void CreateMaterialResource(ID3D12Device* device);
		void UpdateMaterialResource();

		void UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	private:
		static RenderResourceManager* s_render_resource_manager;

		std::vector<SPtr<Mesh>> m_ready_meshes;
		std::vector<SPtr<Material>> m_ready_materials;
		std::vector<SPtr<ExternalTexture>> m_ready_external_textures;
		std::vector<SPtr<RenderTexture>> m_ready_render_textures;

	private:
		UINT m_num_of_material_data = 0;
		UPtr<UploadBuffer<RSMaterialData>> m_material_data;
		UINT m_num_of_texture_data;
		ComPtr<ID3D12DescriptorHeap> m_texture_desciptor_heap;

	public:
		static RenderResourceManager& GetRenderResourceManager() { return *s_render_resource_manager; }
	};
}

