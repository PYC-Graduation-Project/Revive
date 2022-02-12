#pragma once
#include "client/asset/core/asset.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class MeshComponent;
	class Material;
	template<class T> class UploadBuffer;

	struct InstanceInfo
	{
		UINT count;
		UINT start_location;
	};

	struct RSMaterialIndexData
	{
		UINT index;
	};

	enum class eMeshType
	{
		kStaticMesh,
	};

	class Mesh : public Asset
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		virtual void Shutdown();
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const = 0;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT lod) const = 0;
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) { m_primitive_topology = topology; }

		virtual void CreateDataForLodMesh(UINT lod);

	protected:
		eMeshType type;

		UINT m_lod_count = 0;
		std::vector<UINT> m_lod_mesh_counts;

		std::vector<ComPtr<ID3DBlob>> m_vertex_buffer_blobs;
		std::vector<ComPtr<ID3D12Resource>> m_vertex_buffers;
		std::vector<ComPtr<ID3D12Resource>> m_vertex_upload_buffers;

		std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vertex_buffer_views;

		UINT m_slot = 0;

		std::vector<ComPtr<ID3DBlob>> m_index_buffer_blobs;
		std::vector<ComPtr<ID3D12Resource>> m_index_buffers;
		std::vector<ComPtr<ID3D12Resource>> m_index_upload_buffers;

		std::vector<D3D12_INDEX_BUFFER_VIEW> m_index_buffer_views;

		D3D12_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		BOrientedBox m_oriented_box;

		bool m_is_draw_index;

	public:
		UINT GetLODCount() const { return m_lod_count; }
		UINT GetLODMeshCount(UINT lod) const { return m_lod_mesh_counts.at(lod); }
		void AddLODMeshCount(UINT lod) { ++m_lod_mesh_counts.at(lod); }
		ComPtr<ID3DBlob> GetVertexBufferBlob(UINT lod);
		bool CreateVertexBufferBlob(UINT lod, UINT size);
		ComPtr<ID3DBlob> GetIndexBufferBlob(UINT lod);
		bool CreateIndexBufferBlob(UINT lod, UINT size);
		const BOrientedBox& GetOrientedBox() const { return m_oriented_box; }
		void SetOrientBox(const BOrientedBox& oriented_box) { m_oriented_box = oriented_box; }
		bool IsDrawIndex() const { return m_is_draw_index; }
 
	protected:
		std::vector<std::vector<SPtr<Material>>> m_materials;
		std::vector<UPtr<UploadBuffer<RSMaterialIndexData>>> m_material_index_data;

	public:
		void AddMaterial(UINT lod, SPtr<Material>&& material) { m_materials.at(lod).emplace_back(std::move(material)); }
		const std::vector<SPtr<Material>> GetMaterials(UINT lod) const { return m_materials.at(lod); }
	};

	class TextureLightVertex;
	class KDTree;

	class StaticMesh : public Mesh
	{
	public:
		StaticMesh() = default;
		virtual ~StaticMesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT lod) const override;

		virtual void CreateDataForLodMesh(UINT lod) override;

	private:
		std::vector<std::vector<InstanceInfo>> m_instance_info;
		SPtr<KDTree> m_bounding_tree;

	public:
		virtual void AddInstanceInfo(UINT lod, InstanceInfo&& info);
		const SPtr<KDTree>& GetBoundingTree() const { return m_bounding_tree; }
		void SetBoundingTree(SPtr<KDTree>&& tree) { m_bounding_tree = std::move(tree); }
	};
}
