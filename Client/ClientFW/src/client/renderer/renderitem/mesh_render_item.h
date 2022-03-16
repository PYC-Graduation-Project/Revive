#pragma once

namespace client_fw
{
	class Mesh;
	class MeshComponent;
	template<class T> class UploadBuffer;

	struct RSInstanceData
	{
		Mat4 world_matrix;
		Mat4 world_inverse_transpose;
	};

	struct MeshComponentData
	{
		SPtr<MeshComponent> mesh_comp;
		bool is_need_update;
		Mat4 world_transpose;
		Mat4 world_inverse;
	};

	struct MeshData
	{
		SPtr<Mesh> mesh;
		INT draw_start_index;
		std::vector<UINT> index_of_lod_instance_data;
		std::vector<UINT> lod_instance_data;
		std::vector<MeshComponentData> mesh_comp_data;
	};

	class MeshRenderItem final
	{
	public:
		MeshRenderItem();
		~MeshRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list);

		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);

	private:
		void CreateResources(ID3D12Device* device);
		void UpdateResources();
		void UpdateResourcesBeforeDraw(const SPtr<MeshData>& mesh_data, UINT& start_index);

	private:
		bool m_is_need_resource_create = false;

		std::vector<SPtr<MeshData>> m_mesh_data;
		std::map<std::string, SPtr<MeshData>> m_mesh_data_map;

		UPtr<UploadBuffer<RSInstanceData>> m_instance_data;
		UINT m_num_of_instance_data = 0;
		UINT m_real_num_of_instance_data = 0;
	};
}
