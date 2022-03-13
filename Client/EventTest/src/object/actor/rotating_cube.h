#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
	class RotatingMovementComponent;
}

namespace event_test
{
	using namespace client_fw;
	
	class RotatingCube final : public StaticMeshActor
	{
	public:
		RotatingCube();
		virtual ~RotatingCube() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
		virtual void ExecuteMessage(const SPtr<MessageEventInfo>& message) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	private:
		SPtr<RotatingMovementComponent> m_rotating_component;
	};
}



