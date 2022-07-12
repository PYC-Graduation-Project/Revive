#pragma once
#include "object/effect/core/effect_actor.h"

namespace revive
{
	class HitParticle : public EffectActor
	{
	public:
		HitParticle(eMobilityState mobility, const std::string& path, const Vec2& size, bool fix_up);
		virtual ~HitParticle() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;

	private:
		Vec2 m_frame_count = Vec2{2.f,2.f};
		Vec2 m_frame_index = Vec2{0.f,0.f};

		float m_curr_time = 0;

		float m_animation_speed = 1.f;
	public:

		const float GetAnimationSpeed() const { return m_animation_speed; }
		void SetAnimationSpeed(float animation_speed) { m_animation_speed = animation_speed; }

		void SetFrameCount(const float row,const float column) { m_frame_count = Vec2{row,column}; }
		void SetFrameCount(const Vec2& frame_count) { m_frame_count = frame_count; }
	};
}