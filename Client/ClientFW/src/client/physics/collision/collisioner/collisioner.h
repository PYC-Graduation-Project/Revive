#pragma once
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/physics/collision/collision_util.h"

namespace client_fw
{
	class SceneComponent;
	class BoxComponent;
	class StaticMeshComponent;

	enum class eMeshCollisionType
	{
		kStaticMesh, kBox,
	};

	class Collisioner
	{
	public:
		Collisioner(const WPtr<SceneComponent>& owner, eMeshCollisionType type);
		virtual ~Collisioner() = default;

		virtual void CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) = 0;

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
}