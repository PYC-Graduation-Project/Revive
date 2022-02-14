#pragma once
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	class BoxCollisioner : public Collisioner
	{
	public:
		BoxCollisioner(const WPtr<BoxComponent>& owner);
		virtual ~BoxCollisioner() = default;

		virtual void CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) override;
	};
}
