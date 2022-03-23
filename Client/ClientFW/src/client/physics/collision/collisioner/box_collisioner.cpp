#include "stdafx.h"
#include "client/physics/collision/collisioner/box_collisioner.h"
#include "client/object/component/render/box_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	BoxCollisioner::BoxCollisioner()
		: Collisioner(eMeshCollisionType::kBox)
	{
	}

	bool BoxCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		case eMeshCollisionType::kBox:
			return GetOwner()->GetOrientedBox()->Intersects(*other->GetOrientedBox());
		case eMeshCollisionType::kSphere:
		{
			BSphere sphere2(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
			return GetOwner()->GetOrientedBox()->Intersects(sphere2);
		}
		default:
			break;
		}
		return false;
	}
	void BoxCollisioner::BlockOtherComponent(const SPtr<SceneComponent>& other)
	{
	}
}
