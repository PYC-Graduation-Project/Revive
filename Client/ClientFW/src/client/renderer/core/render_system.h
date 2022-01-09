#pragma once

namespace client_fw
{
	enum class eRenderLevelType;
	class GraphicsRenderLevel;
	class GraphicsShader;
	class RenderComponent;
	class Mesh;
	class MeshComponent;
	enum class eRenderComponentType;

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
		bool RegisterGraphicsRenderLevel(const std::string& name)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(), name)
				!= m_render_level_order.cend())
			{
				m_graphics_render_levels[name] = CreateSPtr<T>(name);
				m_graphics_render_levels[name]->Initialize(m_device);
				return true;
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", name);
				return false;
			}
		}

		template <class T>
		bool RegisterGraphicsShader(const std::string& shader_name, const std::string& level_name)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(), level_name)
				!= m_render_level_order.cend())
			{
				m_graphics_shaders[shader_name] = CreateSPtr<T>(shader_name);
				m_graphics_shaders[shader_name]->Initialize(m_device);
				return m_graphics_render_levels[level_name]->RegisterGraphicsShader(m_device, m_graphics_shaders[shader_name]);
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", level_name);
				return false;
			}
		}

		bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp,
			eRenderComponentType comp_type, const std::string& shader_name);
		void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, 
			eRenderComponentType comp_type, const std::string& shader_name);
		bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);

	private:
		ID3D12Device* m_device;

		std::map<std::string, SPtr<GraphicsRenderLevel>> m_graphics_render_levels;
		std::map<std::string, SPtr<GraphicsShader>> m_graphics_shaders;

		std::vector<std::string> m_render_level_order;
		std::unordered_set<std::string> m_initialized_assets; //Level Load Asset시스템을 사용하면, 이것도 초기화 해줘야 한다. 

		std::vector<SPtr<Mesh>> m_ready_meshes;

	public:
		ID3D12Device* GetDevice() const { return m_device; }
		void AddRenderLevelOrder(const std::string& name, const std::string& front_render_level_name);
	};
}
