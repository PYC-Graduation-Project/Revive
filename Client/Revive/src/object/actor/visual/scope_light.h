#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
	class SpotLightComponent;
	class PointLightComponent;
}

namespace revive
{
	using namespace client_fw;
	
	class ScopeLight final : public StaticMeshActor
	{
	public:
		ScopeLight();
		virtual ~ScopeLight() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		SPtr<SpotLightComponent> m_spot_light_component;
		SPtr<PointLightComponent> m_point_light_component;
	};
}


