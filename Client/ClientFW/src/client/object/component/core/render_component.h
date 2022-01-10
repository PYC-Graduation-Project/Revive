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
		RenderComponent(const std::string& name, eRenderComponentType type, int update_order = 100);
		virtual ~RenderComponent() = default;

	protected:
		virtual void RegisterToRenderSystem(const std::string& shader_name);
		virtual void RegisterToRenderSystem(eShaderType shader_type);
		virtual void UnregisterFromRenderSystem(const std::string& shader_name);
		virtual void UnregisterFromRenderSystem(eShaderType shader_type);

	protected:
		eRenderComponentType m_render_type;

	public:
		eRenderComponentType GetRenderType() const { return m_render_type; }
	};
}


