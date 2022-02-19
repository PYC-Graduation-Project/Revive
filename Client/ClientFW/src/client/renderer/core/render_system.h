#pragma once

namespace client_fw
{
	struct Window;
	class GraphicsSuperRootSignature;
	enum class eRenderLevelType;
	class GraphicsRenderLevel;
	class GraphicsShader;
	class RenderComponent;
	class CameraComponent;
	enum class eKindOfRenderLevel;
	class RenderResourceManager;
	class CameraManager;

	class RenderSystem final
	{
	public:
		RenderSystem(const WPtr<Window>& window);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list) const;
		void DrawUI(ID3D12GraphicsCommandList* command_list) const;

		void UpdateViewport();

	public:
		template <class T>
		bool RegisterGraphicsRenderLevel(eRenderLevelType level_type)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(),
				std::make_pair(level_type, eKindOfRenderLevel::kGraphics))
				!= m_render_level_order.cend())
			{
				m_graphics_render_levels[level_type] = CreateSPtr<T>(m_graphics_super_root_signature);
				m_graphics_render_levels[level_type]->Initialize(m_device);
				return true;
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", Render::ConvertRenderLevelType(level_type));
				return false;
			}
		}

		template <class T>
		bool RegisterGraphicsShader(const std::string& shader_name, eRenderLevelType level_type, bool is_custom = false)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(), 
				std::make_pair(level_type, eKindOfRenderLevel::kGraphics))
				!= m_render_level_order.cend())
			{
				m_graphics_shaders[shader_name] = CreateSPtr<T>(shader_name);
				m_graphics_shaders[shader_name]->Initialize(m_device);
				if (is_custom)
					m_added_shaders.insert(shader_name);
				return m_graphics_render_levels[level_type]->RegisterGraphicsShader(m_device, m_graphics_shaders[shader_name]);
			}
			else
			{
				LOG_WARN("Could not find {0} from render system", Render::ConvertRenderLevelType(level_type));
				return false;
			}
		}
		void UnregisterGraphicsShader(const std::string& shader_name, eRenderLevelType level_type);

		bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void SetMainCamera(const SPtr<CameraComponent>& camera_comp);

	private:
		WPtr<Window> m_window;
		ID3D12Device* m_device;

		SPtr<GraphicsSuperRootSignature> m_graphics_super_root_signature;
		std::map<eRenderLevelType, SPtr<GraphicsRenderLevel>> m_graphics_render_levels;
		std::map<std::string, SPtr<GraphicsShader>> m_graphics_shaders;

		std::vector<std::pair<eRenderLevelType, eKindOfRenderLevel>> m_render_level_order;
		std::set<std::string> m_added_shaders;

		UPtr<RenderResourceManager> m_render_asset_manager;
		UPtr<CameraManager> m_camera_manager;


	public:
		ID3D12Resource* GetResource();
		ID3D12Device* GetDevice() const { return m_device; }
	};
}
