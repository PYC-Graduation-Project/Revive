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
		SetScale(1.8f);

		bool ret = StaticMeshActor::Initialize();

		m_spot_light_component->SetLightColor(Vec3(255.0f, 64.f, 96.f) * m_light_intensity);
		m_spot_light_component->SetLocalPosition(Vec3(-29.0f, 30.0f, 0.0f));
		m_spot_light_component->SetLocalRotation(math::ToRadian(-90.0f), 0.0f, 0.0f);
		m_spot_light_component->SetAttenuationRadius(350.0f);
		m_spot_light_component->SetShadowTextureSize(256);
		m_spot_light_component->SetConeOuterAngle(60.f);
#ifdef _DEBUG
		m_spot_light_component->DisableShadow();
#endif
		ret &= AttachComponent(m_spot_light_component);

		UseUpdate();

		return ret;
	}

	void Torch::Shutdown()
	{
	}

	void Torch::Update(float delta_time)
	{
		m_light_intensity += m_light_change_speed * delta_time;

		if (m_light_intensity > s_max_light_intensity)
		{
			m_light_change_speed *= -1.0f;
			m_light_intensity = s_max_light_intensity;
		}

		if (m_light_intensity < 0.0f)
		{
			m_light_change_speed *= -1.0f;
			m_light_intensity = 0.0f;
		}

		m_spot_light_component->SetLightColor(Vec3(255.0f, 64.f, 96.f) * m_light_intensity);
	}

	FenceTorch::FenceTorch(bool use_shadow)
		: StaticMeshActor(eMobilityState::kStatic, "Contents/Visual/fence_torch.obj")
		, m_use_shadow(use_shadow)
	{
		m_point_light_component = CreateSPtr<PointLightComponent>();
	}

	bool FenceTorch::Initialize()
	{
		SetScale(0.35f);

		bool ret = StaticMeshActor::Initialize();

		m_point_light_component->SetLightColor(Vec3(255.0f, 32.f, 0.f) * 50.0f);
		m_point_light_component->SetLocalPosition(Vec3(-80.0f, 120.0f, 0.0f));
		m_point_light_component->SetAttenuationRadius(300.0f);
		m_point_light_component->SetShadowTextureSize(400);
#ifdef _DEBUG
		m_point_light_component->DisableShadow();
#else
		if (m_use_shadow == false)
			m_point_light_component->DisableShadow();
#endif
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