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
	//Healer ���Ϳ� ����� �ְ� ���͸� �Ѱܼ� �����϶�
	class HealEffect
	{
	public:
		HealEffect();
		~HealEffect() = default;

		bool Initialize();
		void Update(float delta_time);

	private:

		//������ �÷��̾ ���� �� ����Ʈ�� ���ֱ����� �߰��� ����
		float m_clean_time = 0.f;

		//particle�� ������ �ڽ�
		WPtr<BoxComponent> m_bounding_box;
		std::vector<SPtr<HealParticle>> m_heal_particles;

		void SpawnActor(const SPtr<Actor>& actor);
		void ResetParticle(const SPtr<HealParticle>& particle);

	public:
		//�ٿ�� �ڽ��� ��������϶�
		void RegisterBox(const SPtr<BoxComponent>& bounding_box);
		//��ƼŬ�� �ϳ��������� �����ϴ� �Լ�
		void CreateEffect();
		//����Ʈ�� ���� ����ϴ� �Լ�
		void CleanEffect();

		//Actor Spawn�ÿ� ��� ��������� ���� ����غ���
		const std::vector<SPtr<HealParticle>>& GetParticles() { return m_heal_particles; }
		
		void SetCleanTime(const float clean_time) { m_clean_time = clean_time; }
		const float GetCleanTime() { return m_clean_time; }
	};
}
