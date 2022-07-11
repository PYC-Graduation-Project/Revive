#pragma once
#include "object/effect/core/effect_actor.h"

namespace revive
{
	class HealParticle : public EffectActor
	{
	public:
		HealParticle(eMobilityState mobility, const std::string& path, const Vec2& size, bool fix_up);
		virtual ~HealParticle() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;
	private:

		float m_life_time = 1.f; //��ƼŬ�� ����
		float m_age = 0.f;	   //��ƼŬ�� ���� ����
		float m_speed = 1.f;	   //��ƼŬ�� �ӵ�

		Vec3 m_velocity;   //��ƼŬ�� �����̴� ����
		//��ġ�� Actor�� �̹� �ִ�

	public:

		float GetLifeTime() { return m_life_time; }
		void SetLifeTime(const float life_time) { m_life_time = life_time; }

		float GetAge() { return m_age; }
		void SetAge(const float age) { m_age = age; }

		float GetSpeed() { return m_speed; }
		void SetSpeed(const float speed) { m_speed = speed; }

		const Vec3& GetVelocity() { return m_velocity; }
		void SetVelocity(const Vec3& velocity) { m_velocity = velocity; }

		bool IsReset() { return (m_life_time <= m_age); }
	};
}