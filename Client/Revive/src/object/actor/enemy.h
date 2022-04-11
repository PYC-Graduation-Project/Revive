#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SkeletalMeshComponent;
	class SphereComponent;
	class BoxComponent;
}

namespace revive
{
	using namespace client_fw;

	class Enemy : public Actor
	{
	public:
		Enemy(const std::string& mesh_path, const std::string& name = "Enemy");
		virtual ~Enemy() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		//virtual void Update(float delta_time) override;

	protected:
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<SphereComponent> m_blocking_sphere; //½Ì±Û
		SPtr<BoxComponent> m_blocking_box; //¸ÖÆ¼

		void FixYPosition();
		virtual bool SetCollisionComponent() { return true; }
	private:
		std::string m_mesh_path;
	
	};
}
