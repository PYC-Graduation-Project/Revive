#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/light/spot_light_component.h>
#include <client/object/component/light/point_light_component.h>

#include "light_tree.h"

namespace revive
{
	LightTree::LightTree()
		: StaticMeshActor(eMobilityState::kStatic, "Contents/Visual/light_tree.obj")
	{

		m_spot_light_component = CreateSPtr<SpotLightComponent>();
	}

	bool LightTree::Initialize()
	{
		SetScale(0.8f);
		bool ret = StaticMeshActor::Initialize();

		m_spot_light_component->SetLightColor(Vec3(116.0f, 227.0f, 246.0f) * 500.0f);
		m_spot_light_component->SetLocalPosition(Vec3(-160.0f, 250.0f, 0.0f));
		m_spot_light_component->SetLocalRotation(math::ToRadian(65.0f), math::ToRadian(-90.0f), 0.0f);
		m_spot_light_component->SetAttenuationRadius(500.0f);
		m_spot_light_component->SetConeOuterAngle(70.f);
		ret &= AttachComponent(m_spot_light_component);

		return ret;
	}

	void LightTree::Shutdown()
	{
	}

	void LightTree::Update(float delta_time)
	{
	}

}