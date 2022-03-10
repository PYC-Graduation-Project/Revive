#pragma once
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	class SphereCollisioner : public Collisioner
	{
	public:
		SphereCollisioner();
		virtual ~SphereCollisioner() = default;

		virtual void CheckCollisionWithOtherComponent(const SPtr<SceneComponent>&other) override;
	};
}


