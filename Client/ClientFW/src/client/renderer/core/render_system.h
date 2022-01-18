#pragma once

namespace client_fw
{
	class GraphicsSuperRootSignature;
	enum class eRenderLevelType;
	class GraphicsRenderLevel;
	class GraphicsShader;
	class RenderComponent;
	class MeshComponent;
	class CameraComponent;
	enum class eKindOfRenderLevel;
	class RenderAssetManager;

	class RenderSystem final
	{
	public:
		RenderSystem();
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Shutdown();

		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list);

	public:
		template <class T>
		bool RegisterGraphicsRenderLevel(eRenderLevelType level_type)
		{
			if (std::find(m_render_level_order.cbegin(), m_render_level_order.cend(),
				std::make_pair(level_type, eKindOfRenderLevel::kGraphics))
				!= m_render_level_order.cend())
			{
				m_graphics_render_levels[level_type] = CreateSPtr<T>(level_type, m_graphics_super_root_signature);
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

		bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name);
		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		ID3D12Device* m_device;
		ID3D12GraphicsCommandList* m_command_list;

		SPtr<GraphicsSuperRootSignature> m_graphics_super_root_signature;
		std::map<eRenderLevelType, SPtr<GraphicsRenderLevel>> m_graphics_render_levels;
		std::map<std::string, SPtr<GraphicsShader>> m_graphics_shaders;

		std::vector<std::pair<eRenderLevelType, eKindOfRenderLevel>> m_render_level_order;
		std::set<std::string> m_added_shaders;

		std::vector<SPtr<CameraComponent>> m_basic_cameras;

		UPtr<RenderAssetManager> m_render_asset_manager;

	public:
		ID3D12Device* GetDevice() const { return m_device; }
	};
}
