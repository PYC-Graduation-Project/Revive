#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include <client/asset/core/asset_store.h>
#include <client/object/component/mesh/core/mesh_component.h>
#include "rect_actor.h"

namespace render_test
{
	RectActor::RectActor()
		: Actor()
	{
	}

	void RectActor::Initialize()
	{
		auto mesh_comp = CreateSPtr<MeshComponent>();
		mesh_comp->SetMesh("../Contents/plane.obj");
		AttachComponent(mesh_comp);
		
		auto rot = GetRotation();
		rot *= quat::CreateQuaternionFromNormal(GetRight(), math::ToRadian(-90.0f));
		SetRotation(rot);

		SetScale(0.005f);
	}

	void RectActor::Shutdown()
	{
	}

	void RectActor::Update(float delta_time)
	{
	}
}
