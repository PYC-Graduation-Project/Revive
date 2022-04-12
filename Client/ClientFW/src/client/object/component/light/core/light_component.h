#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	enum class eLightType
	{
		kDirectional, kPoint, kSpot
	};

	class ShadowCameraComponent;

	class LightComponent : public RenderComponent
	{
	protected:
		LightComponent(eLightType type, const std::string& name,
			const std::string& draw_shader_name);
		virtual ~LightComponent() = default;

		virtual bool Initialize() override;

		virtual void RegisterToVisualOctree() override;
		virtual void UnregisterFromVisualOctree() override;

	protected:
		eLightType m_light_type;
		Vec3 m_light_color = Vec3(1.f, 1.f, 1.f);
		UINT m_light_manager_registered_index = 0;

	public:
		eLightType GetLightType() const { return m_light_type; }
		const Vec3& GetLightColor() const { return m_light_color; }
		void SetLightColor(const Vec3& color) { m_light_color = color; }
		UINT GetLightManagerRegisteredIndex() const { return m_light_manager_registered_index; }
		void SetLightManagerRegisteredIndex(UINT index) { m_light_manager_registered_index = index; }

	protected:
		INT m_shadow_texture_size = 1000;

	public:
		// 초기화 과정이 아닌 Runtime에 호출을 하면 작동하지 않는다.
		INT GetShadowTextureSize() const { return m_shadow_texture_size; }
		void SetShadowTextureSize(INT extent) { m_shadow_texture_size = extent; }

	private:
		virtual void UpdateShadowTextureSize() {}

	protected:
		SPtr<LightComponent> SharedFromThis();
	};
}




