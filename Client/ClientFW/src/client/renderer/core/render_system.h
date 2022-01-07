#pragma once

namespace client_fw
{
	class RenderLevel;
	enum class eRenderLevelType;
	class Shader;

	class RenderSystem final
	{
	public:
		RenderSystem();
		~RenderSystem() = default;

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		bool Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list);

	public:
		template <class T>
		bool RegisterRenderLevel(const std::string& name)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(), name)
				!= m_render_level_order.cend())
			{
				m_render_levels[name] = CreateSPtr<T>(name);
				m_render_levels[name]->Initialize(m_device);
				return true;
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", name);
				return false;
			}
		}

		template <class T>
		bool RegisterShader(const std::string& shader_name, const std::string& level_name)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(), level_name)
				!= m_render_level_order.cend())
			{
				m_shaders[shader_name] = CreateSPtr<T>(shader_name);
				m_shaders[shader_name]->Initialize(m_device);
				return m_render_levels[level_name]->RegisterShader(m_device, m_shaders[shader_name]);
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", level_name);
				return false;
			}
		}

	private:
		ID3D12Device* m_device;

		std::map<std::string, SPtr<RenderLevel>> m_render_levels;
		std::map<std::string, SPtr<Shader>> m_shaders;

		std::vector<std::string> m_render_level_order;

	public:
		ID3D12Device* GetDevice() const { return m_device; }
		void AddRenderLevelOrder(const std::string& name, const std::string& front_render_level_name);
	};
}
