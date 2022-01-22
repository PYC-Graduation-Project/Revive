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

	struct RSMaterialIndexData
	{
		UINT index;
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

		virtual void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);

	private:
		void CreateResources(ID3D12Device* device);
		virtual void UpdateResources();
		void UpdateMaterialIndexResource();

	protected:
		bool m_is_need_resource_create = false;
		SPtr<Mesh> m_mesh;
		UINT m_material_count = 0;
		
		std::vector<MeshComponentData> m_mesh_comp_data;
		std::unordered_set<UINT> m_changed_resource_index;

		UPtr<UploadBuffer<RSInstanceData>> m_instance_data;
		UINT m_num_of_instance_data = 0;

		UPtr<UploadBuffer<RSMaterialIndexData>> m_material_index_data;
		bool m_is_updated_material_index_data = false;

	public:
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
	};
}
