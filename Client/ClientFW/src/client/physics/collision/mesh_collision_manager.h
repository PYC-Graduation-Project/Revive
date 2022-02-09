#pragma once
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/physics/collision/collision_util.h"

namespace client_fw
{
	class Actor;

	enum class eMeshCollisionType
	{
		kStaticMesh
	};

	class MeshCollisionManager : public std::enable_shared_from_this<MeshCollisionManager>
	{
	public:
		MeshCollisionManager(eMeshCollisionType type);
		virtual ~MeshCollisionManager() = default;

	public:
		virtual bool CheckCollisionWithOtherMesh(const SPtr<MeshCollisionManager>& mesh) = 0;
		
	protected:
		CollisionInfo m_collision_info;
		eMeshCollisionType m_type;
		WPtr<Actor> m_owner;

	public:
		eMeshCollisionType GetMeshCollisionType() const { return m_type; }
		const CollisionInfo& GetCollisionInfo() const { return m_collision_info; }
		void SetCollisionInfo(CollisionInfo&& info) { m_collision_info = std::move(info); }
		const WPtr<Actor>& GetOwner() const { return m_owner; }
		void SetOwner(const WPtr<Actor>& owner) { m_owner = owner; }
	};

	class StaticMeshBoundingTree;

	class StaticMeshCollisionManager final : public MeshCollisionManager
	{
	public:
		StaticMeshCollisionManager(const SPtr<StaticMeshBoundingTree>& tree, const SPtr<BOrientedBox>& box);
		~StaticMeshCollisionManager() = default;

	protected:
		virtual bool CheckCollisionWithOtherMesh(const SPtr<MeshCollisionManager>& mesh) override;

	private:
		void CheckCollision(const SPtr<StaticMeshCollisionManager>& mesh);
		bool CheckCollision(const Mat4& mat1, const SPtr<TriTreeNode>& node1,
			const Mat4& mat2, const SPtr<TriTreeNode>& node2, const BOrientedBox& box2);
		bool CheckTriangleCollision(const Mat4& mat1, const SPtr<TriTreeNode>& node1,
			const Mat4& mat2, const SPtr<TriTreeNode>& node2);

	private:
		UINT m_count = 0;
		SPtr<BOrientedBox> m_box;
		SPtr<StaticMeshBoundingTree> m_bounding_tree;

	public:
		const SPtr<BOrientedBox>& GetOrientedBox() const { return m_box; }
		const SPtr<StaticMeshBoundingTree>& GetBoundingTree() const { return m_bounding_tree; }
	};
}


