#pragma once

namespace client_fw
{
	class RenderSystem;
	class RenderLevel;
	class Shader;
	class RenderComponent;
	class MeshComponent;
	class CameraComponent;

	//Render level supported by client framework.
	//The defined order is the execution order.
	enum class eRenderLevelType
	{
		kDefault
	};

	enum class eShaderType
	{
		kDefault,
	};

	enum class eKindOfRenderLevel
	{
		kGraphics, kCompute, kDeferred,
	};

	enum class eRenderComponentType
	{
		kMesh, kLight, kCamera,
	};

	class Render final
	{
	public:
		template <class T>
		static bool RegisterGraphicsRenderLevel(const std::string& name, const std::string& front_render_level_name)
			//if you want to insert to the front, front_render_level_name set blank
		{
			s_render_system->AddRenderLevelOrder(name, front_render_level_name, eKindOfRenderLevel::kGraphics);
			return s_render_system->RegisterGraphicsRenderLevel<T>(name, true);
		}

		template <class T>
		static bool RegisterGraphicsRenderLevel(const std::string& name, eRenderLevelType type)
		{
			return Render::RegisterGraphicsRenderLevel<T>(name, ConvertRenderLevelType(type));
		}

		static void UnregisterGraphicsRenderLevel(const std::string& level_name);

		template <class T>
		static bool RegisterGraphicsShader(const std::string& shader_name, const std::string& level_name)
		{
			return s_render_system->RegisterGraphicsShader<T>(shader_name, level_name, true);
		}

		template <class T>
		static bool RegisterGraphicsShader(const std::string& shader_name, eRenderLevelType type)
		{
			return RegisterGraphicsShader<T>(shader_name, ConvertRenderLevelType(type));
		}

		static void UnregisterGraphicsShader(const std::string shader_name, const std::string& level_name);

		static bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		static void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);

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


