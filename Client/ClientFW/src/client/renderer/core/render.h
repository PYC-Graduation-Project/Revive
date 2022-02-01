#pragma once

namespace client_fw
{
	class RenderSystem;
	class RootSignature;
	class RenderLevel;
	class Shader;
	class RenderComponent;
	class MeshComponent;
	class CameraComponent;


	enum class eRenderLevelType
	{
		kOpaque
	};

	enum class eShaderType
	{
		kOpaqueMesh,
	};

	enum class eKindOfRenderLevel
	{
		kGraphics, kCompute, kDeferred,
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

		static bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		static void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);

		static bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		static void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	public:
		static std::string ConvertRenderLevelType(eRenderLevelType type);
		static std::string ConvertShaderType(eShaderType type);

	private:
		friend RenderSystem;
		static RenderSystem* s_render_system;
	};
}

