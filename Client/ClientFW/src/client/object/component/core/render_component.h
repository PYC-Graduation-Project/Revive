#pragma once
#include "client/object/component/core/component.h"

namespace client_fw
{
	enum class eRenderLevelType;
	enum class eShaderType;
	enum class eRenderComponentType;

	class RenderComponent : public Component, public std::enable_shared_from_this<RenderComponent>
	{
	protected:
		RenderComponent(const std::string& name, eRenderComponentType type,
			const std::string& draw_shader_name, int update_order = 100);
		virtual ~RenderComponent() = default;

	protected:
		virtual bool RegisterToRenderSystem();
		virtual void UnregisterFromRenderSystem();

	protected:
		std::string m_draw_shader_name;
		eRenderComponentType m_render_type;

	public:
		void SetDrawShaderName(const std::string& shader_name){ m_draw_shader_name = shader_name; }
		eRenderComponentType GetRenderType() const { return m_render_type; }
	};
}


