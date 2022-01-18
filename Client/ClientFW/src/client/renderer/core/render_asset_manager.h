#pragma once

namespace client_fw
{
	class Mesh;
	class Material;
	template<class T> class UploadBuffer;

	struct RSMaterialData
	{
		Vec4 base_color;
	};

	class RenderAssetManager final
	{
	public:
		RenderAssetManager();
		~RenderAssetManager();

		void Shutdown();
		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list);

		void RegisterMesh(const SPtr<Mesh>& mesh);

	private:
		void RegisterMaterials(const std::vector<SPtr<Material>>& materials);
		void CreateMaterialResource(ID3D12Device* device);
		void UpdateMaterialResource();

	private:
		std::unordered_set<std::string> m_initialized_assets; //Level Load Asset시스템을 사용하면, 이것도 초기화 해줘야 한다. 

		std::vector<SPtr<Mesh>> m_ready_meshes;
		std::vector<SPtr<Material>> m_ready_materials;

	private:
		UINT m_num_of_material_data = 0;
		UPtr<UploadBuffer<RSMaterialData>> m_material_data;
	};
}

