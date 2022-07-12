#include <include/client_core.h>
#include "object/effect/hit_particle.h"

namespace revive
{
	HitParticle::HitParticle(eMobilityState mobility, 
		const std::string& path, const Vec2& size, bool fix_up)
		:EffectActor(mobility, path, size, fix_up)
	{
		m_name = "Hit " + m_name;

	}
	bool HitParticle::Initialize()
	{
		bool ret = EffectActor::Initialize();

		SetTilling(GetTilling() / m_frame_count);
	
		UseUpdate();
		return ret;
	}
	void HitParticle::Update(float delta_time)
	{
		m_curr_time += delta_time;

		m_frame_index.x = std::round(fmod(m_curr_time * m_animation_speed * delta_time,m_frame_count.x * m_frame_count.y));
		m_frame_index.y = static_cast<float>(static_cast<int>(m_frame_index.x) / static_cast<int>(m_frame_count.y));

		if (m_frame_index.x >= m_frame_count.x * m_frame_count.y - 1)
			SetActorState(eActorState::kDead);
		else
			SetOffset(m_frame_index / m_frame_count);
		
	}
	void HitParticle::Shutdown()
	{
	}
}