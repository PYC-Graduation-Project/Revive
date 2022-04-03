#pragma once
#include "client/object/component/light/core/light_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	class PointLightComponent : public LightComponent
	{
	public:
		PointLightComponent(const std::string& name = "point light component",
			const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kPointLight));
		virtual ~PointLightComponent() = default;

	private:
		virtual void UpdateWorldMatrix() override;
		virtual void UpdateLocalMatrix() override;
		virtual void UpdateOrientedBox() override;

	private:
		float m_attenuation_radius = 1000.0f;

	public:
		float GetAttenuationRadius() const { return m_attenuation_radius; }
		void SetAttenuationRadius(float radius);
	};
}