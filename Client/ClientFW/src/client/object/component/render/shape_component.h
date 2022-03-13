#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class ShapeComponent : public RenderComponent
	{
	protected:
		ShapeComponent(const std::string& name, const Vec3& extents, 
			const std::string& draw_shader_name);
		virtual ~ShapeComponent() = default;

	protected:
		virtual void UpdateOrientedBox() override;

	protected:
		bool m_hidden_in_game = true;
		Vec3 m_extents;

	public:
		bool HiddenInGame() const { return m_hidden_in_game; }
		void SetHiddenInGame(bool value) { m_hidden_in_game = value; }
		const Vec3& GetExtents() const { return m_extents; }
	};
}


