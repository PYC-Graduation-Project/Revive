#pragma once
#include <client/object/actor/core/actor.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>

namespace event_test
{
	using namespace client_fw;

	class Animator : public Actor
	{
	public:
		Animator(const std::string& skeletal_mesh_path, const std::string& animation_name);
		virtual ~Animator() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	private:
		std::string m_mesh_path;
		std::string m_animation_name;

		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
	};


}

