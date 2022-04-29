#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class StaticMeshComponent;
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

	private:
		SPtr<StaticMeshComponent> m_base_mesh;
		SPtr<StaticMeshComponent> m_jewelry_mesh;
	};

}


