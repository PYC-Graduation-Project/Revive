#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class DirectionalLightComponent;

	class DirectionalLight : public Actor
	{
	public:
		DirectionalLight(const std::string& name = "directional light", 
			eMobilityState mobility = eMobilityState::kStatic);
		virtual ~DirectionalLight() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		SPtr<DirectionalLightComponent> m_light_component;

	public:
		const Vec3& GetLightColor() const;
		void SetLightColor(const Vec3& color);
		const Vec3& GetLightDirection() const;
		void SetLightDirection(const Vec3& direction);
	};
}

