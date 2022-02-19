#include "stdafx.h"
#include "client/physics/collision/collisioner/sphere_collisioner.h"
#include "client/object/component/render/sphere_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	SphereCollisioner::SphereCollisioner(const WPtr<SphereComponent>& owner)
		: Collisioner(owner, eMeshCollisionType::kSphere)
	{
	}

	void SphereCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		BSphere sphere1(GetOwner()->GetWorldPosition(), GetOwner()->GetOrientedBox()->GetExtents().x);

		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		case eMeshCollisionType::kBox:
		{
			if (sphere1.Intersects(*other->GetOrientedBox()))
			{
				/*LOG_INFO("{0} col {1}", GetOwner()->GetOwner().lock()->GetName(),
					other->GetOwner().lock()->GetName());*/
			}
			break;
		}
		case eMeshCollisionType::kSphere:
		{
			BSphere sphere2(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
			if (sphere1.Intersects(sphere2))
			{
				/*LOG_INFO("{0} col {1}", GetOwner()->GetOwner().lock()->GetName(),
					other->GetOwner().lock()->GetName());*/
			}
			break;
		}
		default:
			break;
		}
	}
}
