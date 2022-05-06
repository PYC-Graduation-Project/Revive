#include <include/client_core.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "object/actor/test_actor.h"

namespace anim_test
{
	TestActor::TestActor(eMobilityState mobility, const std::string& skeletal_mesh_path, const std::string& animation_name)
		: Actor(mobility), m_mesh_path(skeletal_mesh_path), m_animation_name(animation_name)
	{
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_static_mesh_component = CreateSPtr<StaticMeshComponent>();
	}

	bool TestActor::Initialize()
	{
		bool ret = true;
		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);

		if (m_is_eqquiped)ret &= AttachComponent(m_static_mesh_component);
		SetAnimation(m_animation_name);

		return ret;
	}

	void TestActor::Update(float delta_time)
	{
		if(m_is_eqquiped)
			m_static_mesh_component->SetLocalPosition(m_skeletal_mesh_component->GetSocketWorldPosition(m_socket_name) + m_weapon_offset);
	}

	void TestActor::Shutdown()
	{
		m_skeletal_mesh_component = nullptr;
		m_static_mesh_component = nullptr;
	}

	void TestActor::SetAnimation(const std::string& animation_name)
	{
		m_skeletal_mesh_component->SetAnimation(animation_name);
	}

	void TestActor::SetWeapon(const std::string& mesh_path, const std::string& socket_name)
	{
		//무기클래스에서 해줄것
		m_static_mesh_component->SetMesh(mesh_path);
		m_static_mesh_component->SetLocalScale(0.1f);
		m_is_eqquiped = true;
		m_socket_name = socket_name;
	}
	void TestActor::SetWeaponOffset(const Vec3& offset)
	{
		//무기클래스에서 해줄것
		m_weapon_offset = offset;
		
	}
	SPtr<SkeletalMeshComponent> TestActor::GetSkeletalMeshComponent() const
	{
		return std::static_pointer_cast<SkeletalMeshComponent>(m_skeletal_mesh_component);
	}
}
