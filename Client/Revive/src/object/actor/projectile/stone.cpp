#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "object/actor/projectile/stone.h"

namespace revive
{
	Stone::Stone(const std::string& name)
		:Projectile(name)
	{
	}
	bool Stone::Initialize()
	{
		bool ret = Projectile::Initialize();
		ret &= m_static_mesh_component->SetMesh("Contents/stone_smallH.obj");
		m_static_mesh_component->SetLocalScale(30.f);
		ret &= AttachComponent(m_static_mesh_component);

		SetGravityScale(10.0f);
		return ret;
	}

	void Stone::Update(float delta_time)
	{
		Projectile::Update(delta_time);
		Vec3 current_position = GetPosition();
		if (current_position.y <= 300 + GetSphereRadius())current_position.y = 300.f + GetSphereRadius();
		SetPosition(current_position);
		//LOG_INFO(m_static_mesh_component->GetScale());
	}

	void Stone::SetMeshLocalPosition(const Vec3& position)
	{
		m_static_mesh_component->SetLocalPosition(position);
	}
	
}