#pragma once
#include "client/object/component/core/scene_component.h"

namespace client_fw
{
	enum class eLightType
	{
		kDirectional, kPoint, kSpot
	};

	class LightComponent : public SceneComponent
	{
	protected:
		LightComponent(eLightType type, const std::string& name);
		virtual ~LightComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();

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




