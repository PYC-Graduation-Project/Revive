#pragma once

namespace client_fw
{
	class Mesh;
	class Material;
	class Texture;
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
		void RegisterTexture(const SPtr<Texture>& texture);

	private:
		void RegisterMaterials(const std::vector<SPtr<Material>>& materials);
		void CreateMaterialResource(ID3D12Device* device);
		void UpdateMaterialResource();

		void CreateTextureResource(ID3D12Device* device);
		void UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	private:
		std::unordered_set<std::string> m_initialized_assets; //Level Load Asset시스템을 사용하면, 이것도 초기화 해줘야 한다. 

		std::vector<SPtr<Mesh>> m_ready_meshes;
		std::vector<SPtr<Material>> m_ready_materials;
		std::vector<SPtr<Texture>> m_ready_textures;

	private:
		UINT m_num_of_material_data = 0;
		UPtr<UploadBuffer<RSMaterialData>> m_material_data;
		UINT m_num_of_texture_data;
		ComPtr<ID3D12DescriptorHeap> m_texture_desciptor_heap;

	};
}

