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

	class Torch final : public StaticMeshActor
	{
	public:
		Torch();
		virtual ~Torch() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		SPtr<SpotLightComponent> m_spot_light_component;
	};

	class FenceTorch final : public StaticMeshActor
	{
	public:
		FenceTorch();
		virtual ~FenceTorch() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		SPtr<PointLightComponent> m_point_light_component;
	};
}


