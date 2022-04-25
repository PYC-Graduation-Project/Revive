#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class DirectionalLightComponent;

	class DirectionalLight : public Actor
	{
	public:
		DirectionalLight(eMobilityState mobility = eMobilityState::kStatic,
			const std::string& name = "directional light");
		virtual ~DirectionalLight() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		SPtr<DirectionalLightComponent> m_light_component;

	public:
		const Vec3& GetLightColor() const;
		void SetLightColor(const Vec3& color);

	public:
		//Directional Light는 ~8192까지가 제한, 그 이상은 자동으로 clamp합니다.
		void SetShadowTextureSize(INT extent);
	};
}


