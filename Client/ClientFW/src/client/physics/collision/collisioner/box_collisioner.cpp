#include "stdafx.h"
#include "client/physics/collision/collisioner/box_collisioner.h"
#include "client/object/component/render/box_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	BoxCollisioner::BoxCollisioner(const WPtr<BoxComponent>& owner)
		: Collisioner(owner, eMeshCollisionType::kBox)
	{
	}

	void BoxCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		case eMeshCollisionType::kBox:
		{
			if (GetOwner()->GetOrientedBox()->Intersects(*other->GetOrientedBox()))
			{
				LOG_INFO("{0} col {1}", GetOwner()->GetOwner().lock()->GetName(),
					other->GetOwner().lock()->GetName());
			}
			break;
		}
		case eMeshCollisionType::kSphere:
		{
			BSphere sphere2(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
			if (GetOwner()->GetOrientedBox()->Intersects(sphere2))
			{
				LOG_INFO("{0} col {1}", GetOwner()->GetOwner().lock()->GetName(),
					other->GetOwner().lock()->GetName());
			}
			break;
		}
		default:
			break;
		}
	}
}
