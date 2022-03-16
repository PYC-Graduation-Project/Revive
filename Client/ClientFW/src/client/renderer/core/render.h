#pragma once

namespace client_fw
{
	class RenderSystem;
	class RootSignature;
	class RenderLevel;
	class Shader;
	class RenderComponent;
	class CameraComponent;


	//현재 전략은 Opaque -> Deferred -> Transparent -> Compute -> UI
	//거울같은 반사는 언제 어떻게 처리를 해야 할지.. 
	enum class eRenderLevelType
	{
		kOpaque, kDeferred, kTransparent, kUI, kFinalView
	};

	enum class eShaderType
	{
		kOpaqueMesh, kShapeBox, kTextureBillboard, kOpaqueMaterialBillboard, kOpaqueWidget, kMaskedWidget
	};

	enum class eKindOfRenderLevel
	{
		kGraphics, kCompute
	};

	class Render final
	{
	public:
		template <class T>
		static bool RegisterGraphicsShader(const std::string& shader_name, eRenderLevelType type)
		{
			return s_render_system->RegisterGraphicsShader<T>(shader_name, type, true);
		}

		static void UnregisterGraphicsShader(const std::string shader_name, eRenderLevelType type);

		static bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		static void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);

		static bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		static void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		static void SetMainCamera(const SPtr<CameraComponent>& camera_comp);

	public:
		static Vec2 GetWindowSize();

		static std::string ConvertRenderLevelType(eRenderLevelType type);
		static std::string ConvertShaderType(eShaderType type);

	private:
		friend RenderSystem;
		static RenderSystem* s_render_system;
	};
}


