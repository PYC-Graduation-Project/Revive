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
		float m_light_intensity = 500.0f;
		float m_light_change_speed = 250.0f;

		constexpr static float s_max_light_intensity = 500.0f;

	public:
		void DisableShadow() { m_spot_light_component->DisableShadow(); }
		void SetShadowTextureSize(INT size) { m_spot_light_component->SetShadowTextureSize(size); }
	};

	class FenceTorch final : public StaticMeshActor
	{
	public:
		FenceTorch(bool use_shadow = true);
		virtual ~FenceTorch() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		bool m_use_shadow;
		SPtr<PointLightComponent> m_point_light_component;

	public:
		void DisableShadow() { m_point_light_component->DisableShadow(); }
		void SetShadowTextureSize(INT size) { m_point_light_component->SetShadowTextureSize(size); }
	};
}


