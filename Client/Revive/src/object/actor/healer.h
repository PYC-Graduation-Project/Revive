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

		//이 이펙트 멤버변수를 외부로 빼고싶다면
		//Healer 객체 안에서도 HealEffect를 접근할수 있어야한다.
		//1.WPtr 멤버 변수로 두고 사용한다, 외부에서 Effect 객체 등록, Level에서 업데이트를 해줘야함
		//2.EffectManager:: 형태로 함수를 호출한다. 확장성을 생각하면
		// 이 방법이 제일 좋으나 번거로움(생각해야할 부분이 많다.)
		WPtr<HealEffect> m_heal_effect;

	public:
		void RegisterEffect(const SPtr<HealEffect>& heal_effect) { m_heal_effect = heal_effect; }
	};

}


