#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SphereComponent;
}

namespace revive
{
	using namespace client_fw;

	class ProjectileMovementComponent;
	//LifeSpan 구현 필요 정해진 값(초) 가 지나면 해당액터는 소멸한다.
	//충돌 처리가아니라 레이저 쏴서 거기에 콜리전잇으면 충돌 ㅇㅈ?
	class Projectile : public Actor
	{
	public:
		Projectile(const std::string& name = "Projectile");
		virtual ~Projectile() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		//virtual void Update(float delta_time) override;
	
	private:
		SPtr<SphereComponent> m_sphere_component; //?
		SPtr<StaticMeshComponent> m_static_mesh_component;
		SPtr<ProjectileMovementComponent> m_projectile_movement_component;
	};
}
