#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"

namespace client_fw
{

	void Render::UnregisterGraphicsShader(const std::string shader_name, eRenderLevelType type)
	{
		s_render_system->UnregisterGraphicsShader(shader_name, type);
	}

	bool Render::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		return s_render_system->RegisterMeshComponent(mesh_comp, shader_name);
	}

	void Render::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		s_render_system->UnregisterMeshComponent(mesh_comp, shader_name);
	}

	bool Render::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		return s_render_system->RegisterCameraComponent(camera_comp);
	}

	void Render::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		s_render_system->UnregisterCameraComponent(camera_comp);
	}

	std::string Render::ConvertRenderLevelType(eRenderLevelType type)
	{
		switch (type)
		{
		case eRenderLevelType::kOpaque:
			return "opaque";
		default:
			return "unknown";
		}
	}
	std::string Render::ConvertShaderType(eShaderType type)
	{
		switch (type)
		{
		case eShaderType::kOpaqueMesh:
			return "opaque mesh";
		default:
			return "unknown";
		}
	}
}
