#include "stdafx.h"
#include "client/physics/collision/collisioner/sphere_collisioner.h"
#include "client/object/component/render/sphere_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	SphereCollisioner::SphereCollisioner()
		: Collisioner(eMeshCollisionType::kSphere)
	{
	}

	bool SphereCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		BSphere sphere1(GetOwner()->GetWorldPosition(), GetOwner()->GetOrientedBox()->GetExtents().x);

		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		case eMeshCollisionType::kBox:
			return sphere1.Intersects(*other->GetOrientedBox());
		case eMeshCollisionType::kSphere:
		{
			BSphere sphere2(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
			return sphere1.Intersects(sphere2);
		}
		default:
			break;
		}
		return false;
	}
}
