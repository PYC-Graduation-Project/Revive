#pragma once

namespace client_fw
{
	enum class eRenderLevelType;
	class GraphicsRenderLevel;
	class GraphicsShader;
	class RenderComponent;
	class Mesh;
	class MeshComponent;
	class CameraComponent;
	enum class eRenderComponentType;
	enum class eKindOfRenderLevel;

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
		bool RegisterGraphicsRenderLevel(const std::string& name, bool is_custom = false)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(),
				std::make_pair(name, eKindOfRenderLevel::kGraphics))
				!= m_render_level_order.cend())
			{
				m_graphics_render_levels[name] = CreateSPtr<T>(name);
				m_graphics_render_levels[name]->Initialize(m_device);
				if (is_custom)
					m_added_render_levels.insert(name);
				return true;
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", name);
				return false;
			}
		}
		void UnregisterGraphicsRenderLevel(const std::string& level_name);

		template <class T>
		bool RegisterGraphicsShader(const std::string& shader_name, const std::string& level_name, bool is_custom = false)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(), 
				std::make_pair(level_name, eKindOfRenderLevel::kGraphics))
				!= m_render_level_order.cend())
			{
				m_graphics_shaders[shader_name] = CreateSPtr<T>(shader_name);
				m_graphics_shaders[shader_name]->Initialize(m_device);
				if (is_custom)
					m_added_shaders.insert(shader_name);
				return m_graphics_render_levels[level_name]->RegisterGraphicsShader(m_device, m_graphics_shaders[shader_name]);
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", level_name);
				return false;
			}
		}
		void UnregisterGraphicsShader(const std::string& shader_name, const std::string& level_name);

		bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		ID3D12Device* m_device;

		std::map<std::string, SPtr<GraphicsRenderLevel>> m_graphics_render_levels;
		std::map<std::string, SPtr<GraphicsShader>> m_graphics_shaders;

		std::vector<std::pair<std::string, eKindOfRenderLevel>> m_render_level_order;
		std::set<std::string> m_added_render_levels;
		std::set<std::string> m_added_shaders;
		std::unordered_set<std::string> m_initialized_assets; //Level Load Asset시스템을 사용하면, 이것도 초기화 해줘야 한다. 

		std::vector<SPtr<Mesh>> m_ready_meshes;

		std::vector<SPtr<CameraComponent>> m_basic_cameras;


	public:
		ID3D12Device* GetDevice() const { return m_device; }
		void AddRenderLevelOrder(const std::string& name, const std::string& front_render_level_name, eKindOfRenderLevel kind);
	};
}
