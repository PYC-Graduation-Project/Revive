#include "stdafx.h"
#include "client/physics/collision/collisioner/collisioner.h"
#include "client/object/component/core/scene_component.h"


namespace client_fw
{
	Collisioner::Collisioner(eMeshCollisionType type)
		: m_type(type)
	{
	}

	void Collisioner::SetCollisionInfo(std::string&& collision_type, 
		std::set<std::string>&& collisionable_types, bool generate_collision_event)
	{
		m_collision_info.is_collision = true;
		m_collision_info.collision_type = std::move(collision_type);
		m_collision_info.collisionable_types = std::move(collisionable_types);
		m_collision_info.generate_collision_event = generate_collision_event;
	}
}
