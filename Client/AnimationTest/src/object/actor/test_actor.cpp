#include <include/client_core.h>
#include "object/actor/test_actor.h"

namespace anim_test
{
	TestActor::TestActor(eMobilityState mobility, const std::string& skeletal_mesh_path, const std::string& animation_name)
		: Actor(mobility), m_mesh_path(skeletal_mesh_path), m_animation_name(animation_name)
	{
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
	}

	bool TestActor::Initialize()
	{
		bool ret = true;
		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);

		SetAnimation(m_animation_name);

		return ret;
	}

	void TestActor::Update(float delta_time)
	{
	}

	void TestActor::Shutdown()
	{
		m_skeletal_mesh_component = nullptr;
	}
	void TestActor::SetAnimation(const std::string& animation_name)
	{
		m_skeletal_mesh_component->SetAnimation(animation_name);
	}
}
