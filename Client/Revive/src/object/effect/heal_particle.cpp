#include <include/client_core.h>
#include "object/effect/heal_particle.h"

namespace revive
{
	HealParticle::HealParticle(eMobilityState mobility, 
		const std::string& path, const Vec2& size, bool fix_up)
		:EffectActor(mobility,path,size,fix_up)
	{
		m_name = "Heal " + m_name;
	}
	bool HealParticle::Initialize()
	{
		bool ret = EffectActor::Initialize();

		m_velocity = Vec3{ 0.f,1.f,0.f };
		UseUpdate();

		return ret;
	}
	void HealParticle::Update(float delta_time)
	{
		m_age += delta_time;
		if (m_life_time > m_age)
		{
			SetPosition(GetPosition() + m_velocity * m_speed);
		}

	}
	void HealParticle::Shutdown()
	{
	}

}