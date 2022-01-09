#pragma once

namespace client_fw
{
	class RenderSystem;
	class RenderLevel;
	class Shader;
	class RenderComponent;
	class MeshComponent;

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

	enum class eRenderComponentType
	{
		kMesh, kLight, kCamera,
	};

	class Render final
	{
	public:
		template <class T>
		static bool RegisterRenderLevel(const std::string& name, const std::string& front_render_level_name)
			//if you want to insert to the front, front_render_level_name set blank
		{
			s_render_system->RegisterRenderLevel(name, front_render_level_name);
		}

		template <class T>
		static bool RegisterRenderLevel(const std::string& name, eRenderLevelType type)
		{
			s_render_system->RegisterRenderLevel(name, ConvertRenderLevelType(type));
		}

		template <class T>
		static bool RegisterShader(const std::string& shader_name, const std::string& level_name)
		{
			s_render_system->RegisterShader(shader_name, level_name);
		}

		template <class T>
		static bool RegisterShader(const std::string& shader_name, eRenderLevelType type)
		{
			s_render_system->RegisterShader(shader_name, ConvertRenderLevelType(type));
		}

		static bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		static bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, eShaderType& shader_type);
		static void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		static void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, eShaderType& shader_type);

		static bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		static bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, eShaderType& shader_type);
		static void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		static void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, eShaderType& shader_type);

	private:
		static std::string ConvertRenderLevelType(eRenderLevelType type);
		static std::string ConvertShaderType(eShaderType type);

	private:
		friend RenderSystem;
		static RenderSystem* s_render_system;
	};
}


