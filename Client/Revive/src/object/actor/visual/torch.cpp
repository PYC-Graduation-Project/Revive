#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/light/spot_light_component.h>
#include <client/object/component/light/point_light_component.h>

#include "torch.h"

namespace revive
{
	Torch::Torch()
		: StaticMeshActor(eMobilityState::kStatic, "Contents/Visual/torch.obj")
	{
		m_spot_light_component = CreateSPtr<SpotLightComponent>();
	}

	bool Torch::Initialize()
	{
		SetScale(2.0f);

		bool ret = StaticMeshActor::Initialize();

		m_spot_light_component->SetLightColor(Vec3(255.0f, 32.0f, 0.0f) * 50.0f);
		m_spot_light_component->SetLocalPosition(Vec3(-25.0f, 30.0f, 0.0f));
		m_spot_light_component->SetLocalRotation(math::ToRadian(-90.0f), 0.0f, 0.0f);
		m_spot_light_component->SetAttenuationRadius(500.0f);
		m_spot_light_component->SetConeOuterAngle(70.f);
		m_spot_light_component->DisableShadow();
		ret &= AttachComponent(m_spot_light_component);

		return ret;
	}

	void Torch::Shutdown()
	{
	}

	void Torch::Update(float delta_time)
	{
	}

	FenceTorch::FenceTorch()
		: StaticMeshActor(eMobilityState::kStatic, "Contents/Visual/fence_torch.obj")
	{
		m_point_light_component = CreateSPtr<PointLightComponent>();
	}

	bool FenceTorch::Initialize()
	{
		SetScale(0.35f);

		bool ret = StaticMeshActor::Initialize();

		m_point_light_component->SetLightColor(Vec3(255.0f, 32.0f, 0.0f) * 50.0f);
		m_point_light_component->SetLocalPosition(Vec3(-80.0f, 120.0f, 0.0f));
		m_point_light_component->SetAttenuationRadius(300.0f);
		m_point_light_component->SetShadowTextureSize(512.0f);
		//m_point_light_component->DisableShadow();
		ret &= AttachComponent(m_point_light_component);

		return ret;
	}

	void FenceTorch::Shutdown()
	{
	}

	void FenceTorch::Update(float delta_time)
	{
	}


}