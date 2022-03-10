#pragma once
#include "client/object/component/render/shape_component.h"

namespace client_fw
{
	class BoxComponent : public ShapeComponent
	{
	public:
		BoxComponent(const Vec3& extents = Vec3(32.f, 32.f, 32.f),
			const std::string& name = "box component");
		virtual ~BoxComponent() = default;

	public:
		void SetExtents(const Vec3& extents) { m_extents = extents; }
		void SetExtentX(float x) { m_extents.x = x; }
		void SetExtentY(float y) { m_extents.y = y; }
		void SetExtentZ(float z) { m_extents.z = z; }

	protected:
		SPtr<BoxComponent> SharedFromThis();
	};
}
