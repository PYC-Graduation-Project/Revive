#include <include/client_core.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/box_component.h>
#include "object/actor/Enemy.h"

namespace revive
{
	Enemy::Enemy(const std::string& mesh_path, const std::string& name)
		:Actor(eMobilityState::kMovable,name)
	{
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_blocking_box_component = CreateSPtr<BoxComponent>(Vec3{32.f,32.f,32.f},"Blocking Box");
		m_mesh_path = mesh_path;
	}
	bool Enemy::Initialize()
	{
		bool ret = true;

		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		m_skeletal_mesh_component->SetAnimation("idle");

		m_skeletal_mesh_component->SetName(m_name + " Mesh");
		m_blocking_box_component->SetName(m_name + " " + m_blocking_box_component->GetName());

		return ret;
	}
	void Enemy::Shutdown()
	{
		m_skeletal_mesh_component = nullptr;
		m_blocking_box_component = nullptr;
	}
}