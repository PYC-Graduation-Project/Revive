#include "stdafx.h"
#include "client/physics/collision/collisioner/collisioner.h"
#include "client/object/component/core/scene_component.h"


namespace client_fw
{
	Collisioner::Collisioner(const WPtr<SceneComponent>& owner, eMeshCollisionType type)
		: m_owner(owner), m_type(type)
	{
	}
}
