#pragma once

namespace client_fw
{
	class Shader;
	class GraphicsShader;
	class MeshComponent;
	class CameraComponent;

	class RenderLevel
	{
	public:
		RenderLevel(const std::string& name);
		virtual ~RenderLevel() = default;

		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown() = 0;
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		
	protected:
		virtual bool CreateRootSignature(ID3D12Device* device) = 0;
		virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) = 0;

	protected:
		std::string m_name;
		ComPtr<ID3D12RootSignature> m_root_signature;

	public:
		const std::string& GetName() const { return m_name; }
		const ComPtr<ID3D12RootSignature>& GetRootSignature() const { return m_root_signature; }
	};

	class GraphicsRenderLevel : public RenderLevel, public std::enable_shared_from_this<GraphicsRenderLevel>
	{
	public:
		GraphicsRenderLevel(const std::string& name, std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format);
		virtual ~GraphicsRenderLevel() = default;

		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown();
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void Draw(ID3D12GraphicsCommandList* command_list, const std::vector<SPtr<CameraComponent>>& cameras);

		bool RegisterGraphicsShader(ID3D12Device* device, const SPtr<GraphicsShader>& graphics_shader);
		void UnregisterGraphicsShader(const SPtr<GraphicsShader>& graphics_shader);

	protected:
		virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) {}
		virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) {}
		virtual void SetRootCameraResource(ID3D12GraphicsCommandList* command_list, const SPtr<CameraComponent>& camera) {}

	private:
		std::vector<SPtr<GraphicsShader>> m_graphics_shaders;
		std::vector<DXGI_FORMAT> m_rtv_formats;
		DXGI_FORMAT m_dsv_format;

	public:
		const std::vector<DXGI_FORMAT>& GetRTVFormats() const { return m_rtv_formats; }
		DXGI_FORMAT GetDSVFormat() const { return m_dsv_format; }
	};
	
}



