#include <include/client_core.h>
#include <client/asset/core/asset_manager.h>
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
		std::string parent_path = file_help::GetParentPathFromPath(m_mesh_path);
		std::string stem = file_help::GetStemFromPath(m_mesh_path);
		std::string animation_path = parent_path + "/" + stem + "_" + animation_name + ".rev";
		if (m_animation_name.compare("Null") == 0)
			m_skeletal_mesh_component->SetIsPlaying(false);
		else if(m_animation_name.compare("Null") != 0)
			m_skeletal_mesh_component->SetAnimation(animation_path, stem + "_" + animation_name);
	}
}
