#pragma once

namespace client_fw
{
	class GraphicsSuperRootSignature;
	class Shader;
	class GraphicsShader;
	class MeshComponent;
	class CameraComponent;
	enum class eRenderLevelType;

	class RenderLevel
	{
	public:
		RenderLevel(eRenderLevelType level_type);
		virtual ~RenderLevel() = default;

		virtual bool Initialize(ID3D12Device* device) { return true; }
		virtual void Shutdown() {}
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		
	protected:
		//virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		//virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) = 0;

	protected:
		eRenderLevelType m_level_type;
		bool m_is_custom_root_signature = false;

	public:
		eRenderLevelType GetRenderLevelType() const { return m_level_type; }
	};

	class GraphicsRenderLevel : public RenderLevel, public std::enable_shared_from_this<GraphicsRenderLevel>
	{
	public:
		GraphicsRenderLevel(eRenderLevelType level_type, const SPtr<GraphicsSuperRootSignature>& root_signature,
			std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format);
		virtual ~GraphicsRenderLevel() = default;

		virtual void Shutdown();
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void Draw(ID3D12GraphicsCommandList* command_list, const std::vector<SPtr<CameraComponent>>& cameras);

		bool RegisterGraphicsShader(ID3D12Device* device, const SPtr<GraphicsShader>& graphics_shader);
		void UnregisterGraphicsShader(const SPtr<GraphicsShader>& graphics_shader);

	protected:
		//virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) {}
		//virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) {}
		virtual void SetRootCameraResource(ID3D12GraphicsCommandList* command_list, const SPtr<CameraComponent>& camera);

	private:
		SPtr<GraphicsSuperRootSignature> m_graphics_root_signature;
		std::vector<SPtr<GraphicsShader>> m_graphics_shaders;
		std::vector<DXGI_FORMAT> m_rtv_formats;
		DXGI_FORMAT m_dsv_format;

	public:
		const std::vector<DXGI_FORMAT>& GetRTVFormats() const { return m_rtv_formats; }
		DXGI_FORMAT GetDSVFormat() const { return m_dsv_format; }
		const SPtr<GraphicsSuperRootSignature>& GetRootSignature() const { return m_graphics_root_signature; }
	};
	
}


