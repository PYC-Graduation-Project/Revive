#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/object/component/core/render_component.h"
//#include "client/renderer/renderlevel/core/render_level.h"
//#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	bool Render::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		return s_render_system->RegisterRenderComponent(render_comp, 
			render_comp->GetRenderType(), shader_name);
	}

	bool Render::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, eShaderType& shader_type)
	{
		return s_render_system->RegisterRenderComponent(render_comp, 
			render_comp->GetRenderType(), ConvertShaderType(shader_type));
	}

	void Render::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		s_render_system->UnregisterRenderComponent(render_comp,
			render_comp->GetRenderType(), shader_name);
	}

	void Render::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, eShaderType& shader_type)
	{
		s_render_system->UnregisterRenderComponent(render_comp,
			render_comp->GetRenderType(), ConvertShaderType(shader_type));
	}

	bool Render::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		return s_render_system->RegisterMeshComponent(mesh_comp, shader_name);
	}

	bool Render::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, eShaderType& shader_type)
	{
		return s_render_system->RegisterMeshComponent(mesh_comp, ConvertShaderType(shader_type));
	}

	void Render::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		s_render_system->UnregisterMeshComponent(mesh_comp, shader_name);
	}

	void Render::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, eShaderType& shader_type)
	{
		s_render_system->UnregisterMeshComponent(mesh_comp, ConvertShaderType(shader_type));
	}

	std::string Render::ConvertRenderLevelType(eRenderLevelType type)
	{
		switch (type)
		{
		case eRenderLevelType::kDefault:
			return "default";
		default:
			return "unknown";
		}
	}
	std::string Render::ConvertShaderType(eShaderType type)
	{
		switch (type)
		{
		case eShaderType::kDefault:
			return "default";
		default:
			return "unknown";
		}
	}
}
