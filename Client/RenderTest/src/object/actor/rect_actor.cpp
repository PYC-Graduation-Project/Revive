#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include <client/asset/core/asset_store.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "object/actor/rect_actor.h"

namespace render_test
{
	RectActor::RectActor()
		: StaticMeshActor(eMobilityState::kDestructable, "../Contents/plane.obj")
	{
		m_static_mesh_component->SetDrawShaderName("render rect shader");
	}

	bool RectActor::Initialize()
	{
		if (StaticMeshActor::Initialize() == false)
			return false;

		auto rot = GetRotation();
		rot *= quat::CreateQuaternionFromNormal(GetRight(), math::ToRadian(-90.0f));
		SetRotation(rot);

		SetScale(0.005f);

		return true;
	}

	void RectActor::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void RectActor::Update(float delta_time)
	{
	}
}
