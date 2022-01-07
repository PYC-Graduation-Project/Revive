#pragma once

namespace client_fw
{
	class RenderSystem;
	class RenderLevel;
	class Shader;

	//Render level supported by client framework.
	//The defined order is the execution order.
	enum class eRenderLevelType
	{
		kDefault
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

	private:
		static std::string ConvertRenderLevelType(eRenderLevelType type);

	private:
		friend RenderSystem;
		static RenderSystem* s_render_system;
	};
}


