#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class RenderCameraComponent;
}

namespace revive
{
	using namespace client_fw;

	class CameraActor : public Actor
	{
	public:
		CameraActor();
		virtual ~CameraActor() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:

		SPtr<RenderCameraComponent> m_camera_component;

	public:

		void SetMainCamera();
	};
}

