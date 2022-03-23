#pragma once
#include "client/object/component/light/core/light_component.h"

namespace client_fw
{
	class DirectionalLightComponent : public LightComponent
	{
	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent() = default;

	private:
		Vec3 m_direction;

	public:
		const Vec3& GetDirection() const { return m_direction; }
		void SetDirection(const Vec3& direction) { m_direction = direction; }
	};
}


