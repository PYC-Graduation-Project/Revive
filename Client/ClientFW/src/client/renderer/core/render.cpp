#include "stdafx.h"
#include "client/core/window.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"

namespace client_fw
{

	void Render::UnregisterGraphicsShader(const std::string shader_name, eRenderLevelType type)
	{
		s_render_system->UnregisterGraphicsShader(shader_name, type);
	}

	bool Render::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		return s_render_system->RegisterRenderComponent(render_comp, shader_name);
	}

	void Render::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		s_render_system->UnregisterRenderComponent(render_comp, shader_name);
	}

	bool Render::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		return s_render_system->RegisterCameraComponent(camera_comp);
	}

	void Render::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		s_render_system->UnregisterCameraComponent(camera_comp);
	}

	void Render::SetMainCamera(const SPtr<CameraComponent>& camera_comp)
	{
		s_render_system->SetMainCamera(camera_comp);
	}

	Vec2 Render::GetWindowSize()
	{
		const auto& window = s_render_system->GetWindow();
		return Vec2(static_cast<float>(window->width), static_cast<float>(window->height));
	}

	std::string Render::ConvertRenderLevelType(eRenderLevelType type)
	{
		switch (type)
		{
		case eRenderLevelType::kOpaque:
			return "opaque";
		case eRenderLevelType::kTransparent:
			return "transparent";
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
		case eShaderType::kShapeBox:
			return "shape box";
		case eShaderType::kTextureBillboard:
			return "texture billboard";
		case eShaderType::kOpaqueMaterialBillboard:
			return "opaque material billboard";
		case eShaderType::kOpaqueWidget:
			return "opaque widget";
		case eShaderType::kMaskedWidget:
			return "masked widget";
		case eShaderType::kSkeletalMesh:
			return "skeletal mesh";
		default:
			return "unknown";
		}
	}
}
