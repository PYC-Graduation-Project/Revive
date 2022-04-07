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
	//LifeSpan ���� �ʿ� ������ ��(��) �� ������ �ش���ʹ� �Ҹ��Ѵ�.
	//�浹 ó�����ƴ϶� ������ ���� �ű⿡ �ݸ��������� �浹 ����?
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
