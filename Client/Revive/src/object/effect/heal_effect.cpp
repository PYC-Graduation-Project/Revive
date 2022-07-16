#include <include/client_core.h>
#include <client/object/component/render/box_component.h>
#include "client/object/level/core/level_manager.h"

#include "object/effect/core/effect_actor.h"
#include "object/effect/heal_particle.h"
#include "object/effect/heal_effect.h"
#include "util/revive_math.h"

namespace revive
{
	HealEffect::HealEffect()
	{
	}
	bool HealEffect::Initialize()
	{
		return true;
	}

	void HealEffect::Update(float delta_time)
	{
		m_clean_time += delta_time;

		if (m_clean_time > 3.f) CleanEffect();

		if (m_heal_particles.empty() == false)
		{
			for (auto particle : m_heal_particles)
			{
				if (particle->IsReset())
					ResetParticle(particle);
			}
		}
	}

	void HealEffect::SpawnActor(const SPtr<Actor>& actor)
	{
		LevelManager::GetLevelManager().SpawnActor(actor);
	}

	void HealEffect::ResetParticle(const SPtr<HealParticle>& particle)
	{
		//박스 정보를 파티클마다 주기 싫어서 파티클 내부가아니라 외부에서 초기화해주었다.
		auto box = m_bounding_box.lock();
		particle->SetPosition(revive_math::RandomPointInBox(box->GetWorldPosition(), box->GetExtents()));
		particle->SetSpeed(static_cast<float>(rand() % 3 + 1));
		particle->SetLifeTime(static_cast<float>(rand() % 3 + 1));
		particle->SetAge(0.f);
	}

	void HealEffect::RegisterBox(const SPtr<BoxComponent>& bounding_box)
	{
		m_bounding_box = bounding_box;
	}

	void HealEffect::CreateEffect()
	{
		while(m_heal_particles.size() <= MAX_PARTICLE)
		{
			auto heal_particle = CreateSPtr<HealParticle>(eMobilityState::kDestructible,
				"Contents/effect/heal_particle.dds", Vec2{ 30.f,30.f }, true);
			ResetParticle(heal_particle);
			SpawnActor(heal_particle);
			m_heal_particles.push_back(heal_particle);
		}
	}

	void HealEffect::CleanEffect()
	{
		if(m_heal_particles.empty() == false)
		{
			for (const auto& particle : m_heal_particles)
				particle->SetActorState(eActorState::kDead);
			m_heal_particles.clear();
		}
	}

	
}