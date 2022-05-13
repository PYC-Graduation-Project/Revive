#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
	class SpotLightComponent;
}

namespace revive
{
	using namespace client_fw;

	class LightTree final : public StaticMeshActor
	{
	public:
		LightTree();
		virtual ~LightTree() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		SPtr<SpotLightComponent> m_spot_light_component;

	public:
		void DisableShadow() { m_spot_light_component->DisableShadow(); }
		void SetShadowTextureSize(INT size) { m_spot_light_component->SetShadowTextureSize(size); }
	};

}

