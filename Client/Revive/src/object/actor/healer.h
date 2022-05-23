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
	};

}


