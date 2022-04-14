#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class SpotLightComponent;

	class SpotLight : public Actor
	{
	public:
		SpotLight(eMobilityState mobility = eMobilityState::kStatic,
			const std::string& name = "spot light");
		virtual ~SpotLight() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		SPtr<SpotLightComponent> m_spot_light_component;

	public:
		const Vec3& GetLightColor() const;
		void SetLightColor(const Vec3& color);
		float GetAttenuationRadius() const;
		void SetAttenuationRadius(float radius);
		float GetConeInnerAngle() const;
		void SetConeInnerAngle(float degree);
		float GetConeOuterAngle() const;
		void SetConeOuterAngle(float degree);

	public:
		//Spot Light는 ~2048까지가 제한 그 이상은 자동으로 clamp합니다.
		void SetShadowTextureSize(INT extent);
	};

}

