#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include <client/asset/core/asset_store.h>
#include "object/actor/rect_actor.h"

namespace render_test
{
	RectActor::RectActor()
		: StaticMeshActor(eMobilityState::kDestructable, "../Contents/plane.obj")
	{
	}

	void RectActor::Initialize()
	{
		StaticMeshActor::Initialize();

		auto rot = GetRotation();
		rot *= quat::CreateQuaternionFromNormal(GetRight(), math::ToRadian(-90.0f));
		SetRotation(rot);

		SetScale(0.005f);
	}

	void RectActor::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void RectActor::Update(float delta_time)
	{
	}
}
