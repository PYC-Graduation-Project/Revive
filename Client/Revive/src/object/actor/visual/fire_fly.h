#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
    class PointLightComponent;
	class SimpleMovementComponent;
}

namespace revive
{
    using namespace client_fw;

    class FireFly final : public StaticMeshActor
    {
	public:
		FireFly();
		virtual ~FireFly() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		SPtr<PointLightComponent> m_point_light_component;
		SPtr<SimpleMovementComponent> m_simple_movement_component;

	public:
		void SetLightColor(const Vec3& color) { m_point_light_component->SetLightColor(color * 5000.0f); }
    };
}


