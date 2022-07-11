#pragma once

namespace client_fw
{
	class BoxComponent;
}

namespace revive
{
	using namespace client_fw;

#define MAX_PARTICLE 30

	class HealParticle;
	//Healer 액터에 멤버로 넣고 벡터를 넘겨서 스폰하라
	class HealEffect
	{
	public:
		HealEffect();
		~HealEffect() = default;

		bool Initialize();
		void Update(float delta_time);

	private:

		//힐존에 플레이어가 없을 때 이펙트를 없애기위해 추가한 변수
		float m_clean_time = 0.f;

		//particle을 생성할 박스
		WPtr<BoxComponent> m_bounding_box;
		std::vector<SPtr<HealParticle>> m_heal_particles;

		void SpawnActor(const SPtr<Actor>& actor);
		void ResetParticle(const SPtr<HealParticle>& particle);

	public:
		//바운딩 박스를 먼저등록하라
		void RegisterBox(const SPtr<BoxComponent>& bounding_box);
		//파티클이 하나도없으면 생성하는 함수
		void CreateEffect();
		//이펙트를 끌때 사용하는 함수
		void CleanEffect();

		//Actor Spawn시에 사용 스폰방식을 좀더 고민해볼것
		const std::vector<SPtr<HealParticle>>& GetParticles() { return m_heal_particles; }
		
		void SetCleanTime(const float clean_time) { m_clean_time = clean_time; }
		const float GetCleanTime() { return m_clean_time; }
	};
}
