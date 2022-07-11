#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class StaticMeshComponent;
	class BoxComponent;
}

namespace revive
{
	using namespace client_fw;

	class HealEffect;

	class Healer : public Actor
	{
	public:
		Healer();
		virtual ~Healer() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	private:
		SPtr<StaticMeshComponent> m_base_mesh;
		SPtr<StaticMeshComponent> m_jewelry_mesh;
		SPtr<BoxComponent> m_collision_box;
		SPtr<BoxComponent> m_heal_box;

		//�� ����Ʈ ��������� �ܺη� ����ʹٸ�
		//Healer ��ü �ȿ����� HealEffect�� �����Ҽ� �־���Ѵ�.
		//1.WPtr ��� ������ �ΰ� ����Ѵ�, �ܺο��� Effect ��ü ���, Level���� ������Ʈ�� �������
		//2.EffectManager:: ���·� �Լ��� ȣ���Ѵ�. Ȯ�强�� �����ϸ�
		// �� ����� ���� ������ ���ŷο�(�����ؾ��� �κ��� ����.)
		WPtr<HealEffect> m_heal_effect;

	public:
		void RegisterEffect(const SPtr<HealEffect>& heal_effect) { m_heal_effect = heal_effect; }
	};

}


