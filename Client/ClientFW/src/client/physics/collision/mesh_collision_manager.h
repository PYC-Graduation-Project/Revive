#pragma once
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/physics/collision/collision_util.h"

namespace client_fw
{
	class Actor;
	class MeshComponent;

	enum class eMeshCollisionType
	{
		kStaticMesh
	};

	class MeshCollisionManager : public std::enable_shared_from_this<MeshCollisionManager>
	{
	public:
		MeshCollisionManager(const WPtr<MeshComponent>& mesh, eMeshCollisionType type);
		virtual ~MeshCollisionManager() = default;

	public:
		virtual bool CheckCollisionWithOtherMesh(const SPtr<MeshCollisionManager>& mesh) = 0;
		
	protected:
		CollisionInfo m_collision_info;
		eMeshCollisionType m_type;
		WPtr<MeshComponent> m_mesh;

	public:
		eMeshCollisionType GetMeshCollisionType() const { return m_type; }
		const CollisionInfo& GetCollisionInfo() const { return m_collision_info; }
		void SetCollisionInfo(CollisionInfo&& info) { m_collision_info = std::move(info); }
	};

	class KDTree;

	class StaticMeshCollisionManager final : public MeshCollisionManager
	{
	public:
		StaticMeshCollisionManager(const WPtr<MeshComponent>& mesh, const SPtr<KDTree>& tree, const SPtr<BOrientedBox>& box);
		~StaticMeshCollisionManager() = default;

	protected:
		virtual bool CheckCollisionWithOtherMesh(const SPtr<MeshCollisionManager>& mesh) override;

	private:
		void CheckCollision(const SPtr<StaticMeshCollisionManager>& mesh);
		bool CheckCollision(const Mat4& mat1, const SPtr<KDTree>& tree1, const SPtr<KDTreeNode>& node1, const SPtr<MeshComponent>& mesh1,
			const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2, const SPtr<MeshComponent>& mesh2, const BOrientedBox& box2);
		bool CheckCollision(const BOrientedBox& box1, const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2);
		bool CheckTriangleCollision(const Mat4& mat1, const SPtr<MeshComponent>& mesh1, const SPtr<KDTreeNode>& node1,
			const Mat4& mat2, const SPtr<MeshComponent>& mesh2, const SPtr<KDTreeNode>& node2);

	private:
		UINT m_node_count = 0, m_tri_count = 0;
		SPtr<BOrientedBox> m_box;
		SPtr<KDTree> m_bounding_tree;

	public:
		const SPtr<BOrientedBox>& GetOrientedBox() const { return m_box; }
		const SPtr<KDTree>& GetBoundingTree() const { return m_bounding_tree; }
	};
}


