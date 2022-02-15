#include "stdafx.h"
#include "object/actor/test_actor.h"

namespace anim_test
{
	TestActor::TestActor(eMobilityState mobility, const std::string& static_mesh_path)
		: Actor(mobility), m_mesh_path(static_mesh_path)
	{
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
	}

	bool TestActor::Initialize()
	{
		bool ret = true;
		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);
		return ret;
	}

	void TestActor::Shutdown()
	{
		m_skeletal_mesh_component = nullptr;
	}
}
