#pragma once
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/physics/collision/collision_util.h"

namespace client_fw
{
	class SceneComponent;
	class StaticMeshComponent;

	enum class eMeshCollisionType
	{
		kStaticMesh
	};

	class Collisioner
	{
	public:
		Collisioner(const WPtr<SceneComponent>& owner, eMeshCollisionType type);
		virtual ~Collisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) = 0;

	protected:
		CollisionInfo m_collision_info;
		eMeshCollisionType m_type;
		WPtr<SceneComponent> m_owner;

	public:
		eMeshCollisionType GetMeshCollisionType() const { return m_type; }
		const CollisionInfo& GetCollisionInfo() const { return m_collision_info; }
		void SetCollisionInfo(CollisionInfo&& info) { m_collision_info = std::move(info); }
		SPtr<SceneComponent> GetOwner() const { return m_owner.lock(); }
	};

	class StaticMesh;
	class KDTree;
	
	class StaticMeshCollisioner : public Collisioner
	{
	public:
		StaticMeshCollisioner(const WPtr<StaticMeshComponent>& owner);
		virtual ~StaticMeshCollisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) override;

	private:
		void CheckCollsionWithStaticMesh(const SPtr<StaticMeshComponent>& mesh1, eCollisionComplex complex1, const SPtr<StaticMeshComponent>& mesh2, eCollisionComplex complex2);
		bool CheckCollision(const Mat4& mat1, const SPtr<KDTree>& tree1, const SPtr<KDTreeNode>& node1, const SPtr<StaticMeshComponent>& mesh1,
			const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2, const SPtr<StaticMeshComponent>& mesh2, const BOrientedBox& box2);
		bool CheckCollision(const BOrientedBox& box1, const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2);
		bool CheckTriangleCollision(const Mat4& mat1, const SPtr<StaticMeshComponent>& mesh1, const SPtr<KDTreeNode>& node1,
			const Mat4& mat2, const SPtr<StaticMeshComponent>& mesh2, const SPtr<KDTreeNode>& node2);

	private:
		UINT m_node_count = 0, m_tri_count = 0;
	};



}