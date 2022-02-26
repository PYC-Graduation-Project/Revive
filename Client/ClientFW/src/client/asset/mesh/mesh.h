#pragma once
#include "client/asset/core/asset.h"
#include "client/asset/primitive/primitive.h"
#include "client/asset/mesh/mesh_loader.h"
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

	class Mesh : public Asset, public Primitive
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual void Shutdown() override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const = 0;
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

		virtual void CreateDataForLodMesh(UINT lod);

	protected:
		UINT m_lod_count = 0;
		std::vector<UINT> m_lod_mesh_counts;

		std::vector<SPtr<VertexInfo>> m_vertex_infos;
		std::vector<SPtr<IndexInfo>> m_index_infos;

		BOrientedBox m_oriented_box;

		bool m_is_draw_index = false;

	public:
		UINT GetLODCount() const { return m_lod_count; }
		UINT GetLODMeshCount(UINT lod) const { return m_lod_mesh_counts.at(lod); }
		void AddLODMeshCount(UINT lod) { ++m_lod_mesh_counts.at(lod); }
		const SPtr<VertexInfo>& GetVertexInfo(UINT lod) const { return m_vertex_infos.at(lod); }
		const SPtr<IndexInfo>& GetIndexInfo(UINT lod) const { return m_index_infos.at(lod); }
		const BOrientedBox& GetOrientedBox() const { return m_oriented_box; }
		void SetOrientBox(const BOrientedBox& oriented_box) { m_oriented_box = oriented_box; }
		void SetDrawIndex(bool value) { m_is_draw_index = value; }
		bool IsDrawIndex() const { return m_is_draw_index; }
 
	protected:
		std::vector<std::vector<InstanceInfo>> m_instance_info;
		std::vector<std::vector<SPtr<Material>>> m_materials;
		std::vector<UPtr<UploadBuffer<RSMaterialIndexData>>> m_material_index_data;

	public:
		virtual void AddInstanceInfo(UINT lod, InstanceInfo&& info);
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

		SPtr<KDTree> m_bounding_tree;

	public:
		TextureLightVertex* GetVertices(UINT lod) const { return m_vertex_infos.at(lod)->GetVertices<TextureLightVertex>(); }
		UINT GetVertexCount(UINT lod) const { return m_vertex_infos.at(lod)->GetVertexCount<TextureLightVertex>(); }
		const SPtr<KDTree>& GetBoundingTree() const { return m_bounding_tree; }
		void SetBoundingTree(SPtr<KDTree>&& tree) { m_bounding_tree = std::move(tree); }
	
	};

	struct BoneData
	{
		std::vector<std::string> bone_names;
		std::vector<Mat4> bone_offsets;
		std::vector<IVec4> bone_indices;
		std::vector<Vec4> bone_weights;
		std::vector<BOrientedBox> oriented_boxes;
	};

	struct AnimationData //AnimationData
	{
		std::array<Mat4, 128> mat128;
	};

	class SkeletalMesh : public Mesh
	{
	public:
		SkeletalMesh() = default;
		virtual ~SkeletalMesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT lod) const override;


	private:
		SPtr<Skeleton> m_skeleton;
		std::vector<SPtr<Skeleton>> m_cache_skeleton;

		SPtr<BoneData> m_bone_data;

		std::vector<UPtr<UploadBuffer<IVec4>>> m_bone_index_data;
		std::vector<UPtr<UploadBuffer<Vec4>>> m_bone_weight_data;

		ComPtr<ID3D12Resource> m_bone_index_buffer;
		ComPtr<ID3D12Resource> m_bone_index_upload_buffer;

		D3D12_VERTEX_BUFFER_VIEW m_bone_index_buffer_view;
		
		ComPtr<ID3D12Resource> m_bone_weight_buffer;
		ComPtr<ID3D12Resource> m_bone_weight_upload_buffer;

		D3D12_VERTEX_BUFFER_VIEW m_bone_weight_buffer_view;

		std::vector<UPtr<UploadBuffer<AnimationData>>> m_bone_offset_data;
		std::vector<UPtr<UploadBuffer<AnimationData>>> m_bone_trans_data;
		bool m_is_need_init_offset = false;
		void UpdateBoneTransform() const;

	public:
		const SPtr<Skeleton>& GetSkeleton() { return m_skeleton; }

		void SetSkeleton(SPtr<Skeleton>& skeleton) { m_skeleton = skeleton; }

		void SetBoneData(const SPtr<BoneData>& bone_data) { m_bone_data = bone_data; }
	
		void SetAnimatedSkeleton(const std::vector<SPtr<Skeleton>>& animated_skeleton) {m_cache_skeleton = animated_skeleton;}
		
	};
}
