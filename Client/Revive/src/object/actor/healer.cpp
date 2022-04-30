#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/light/point_light_component.h>
#include <client/asset/mesh/mesh.h>
#include <client/asset/material/material.h>

#include "healer.h"

namespace revive
{
	Healer::Healer()
		: Actor(eMobilityState::kStatic, "Healer")
	{
		m_base_mesh = CreateSPtr<StaticMeshComponent>();
		m_jewelry_mesh = CreateSPtr<StaticMeshComponent>();
	}

	bool Healer::Initialize()
	{
		SetRotation(0.0f, math::ToRadian(180.0f), 0.0f);

		bool ret = true;
		ret &= m_base_mesh->SetMesh("Contents/visual/heal_base.obj");
		ret &= m_jewelry_mesh->SetMesh("Contents/visual/heal_jewelry.obj");

		for (const auto& material : m_jewelry_mesh->GetStaticMesh()->GetMaterials(0))
		{
			material->SetMetallic(1.0f);
			material->SetRoughness(0.2f);
		}

		ret &= AttachComponent(m_base_mesh);
		ret &= AttachComponent(m_jewelry_mesh);

		return ret;
	}

	void Healer::Shutdown()
	{
	}

	void Healer::Update(float delta_time)
	{
	}

}