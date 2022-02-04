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

	class RenderItem final
	{
	public:
		RenderItem(const SPtr<Mesh>& mesh);
		~RenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list);

		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);

	private:
		void CreateResources(ID3D12Device* device);
		void UpdateResources();
		void UpdateResourcesBeforeDraw();

	protected:
		bool m_is_need_resource_create = false;
		SPtr<Mesh> m_mesh;
		std::vector<UINT> m_index_of_lod_instance_data;

		std::vector<MeshComponentData> m_mesh_comp_data;
		std::unordered_set<UINT> m_changed_resource_index;

		UPtr<UploadBuffer<RSInstanceData>> m_instance_data;
		UINT m_num_of_instance_data = 0;

	public:
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
	};
}
