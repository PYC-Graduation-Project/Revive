#include <include/client_core.h>
#include "animator.h"

namespace event_test
{
	Animator::Animator(const std::string& skeletal_mesh_path, const std::string& animation_name)
		: Actor(eMobilityState::kMovable)
		, m_mesh_path(skeletal_mesh_path), m_animation_name(animation_name)
	{
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
	}

	bool Animator::Initialize()
	{
		bool ret = m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetAnimation(m_animation_name);

		return ret;
	}

	void Animator::Update(float delta_time)
	{
	}

	void Animator::Shutdown()
	{
	}
}