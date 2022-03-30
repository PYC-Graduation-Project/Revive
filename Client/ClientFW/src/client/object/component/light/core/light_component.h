#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	enum class eLightType
	{
		kDirectional, kPoint, kSpot
	};

	class LightComponent : public RenderComponent
	{
	protected:
		LightComponent(eLightType type, const std::string& name,
			const std::string& draw_shader_name);
		virtual ~LightComponent() = default;

		virtual void RegisterToVisualOctree();
		virtual void UnregisterFromVisualOctree();

	protected:
		eLightType m_light_type;
		Vec3 m_light_color = Vec3(1.f, 1.f, 1.f);

	public:
		eLightType GetLightType() const { return m_light_type; }
		const Vec3& GetLightColor() const { return m_light_color; }
		void SetLightColor(const Vec3& color) { m_light_color = color; }

	protected:
		SPtr<LightComponent> SharedFromThis();
	};
}




