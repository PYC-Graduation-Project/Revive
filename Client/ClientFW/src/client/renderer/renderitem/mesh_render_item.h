#pragma once

namespace client_fw
{
	class Mesh;
	class MeshComponent;
	struct RSInstanceData;

	struct MeshData
	{
		SPtr<Mesh> mesh;
		std::vector<SPtr<MeshComponent>> mesh_comps;
	};

	class MeshRenderItem final
	{
	public:
		MeshRenderItem();
		~MeshRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device);
		void UpdateFrameResource(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list);

		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);

	private:
		std::vector<SPtr<MeshData>> m_mesh_data;
		std::map<std::string, SPtr<MeshData>> m_mesh_data_map;

		std::vector<RSInstanceData> m_meshes_instance_data;
	};
}
