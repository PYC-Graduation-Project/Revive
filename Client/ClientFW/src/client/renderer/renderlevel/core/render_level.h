#pragma once

namespace client_fw
{
	class Shader;

	class RenderLevel : public std::enable_shared_from_this<RenderLevel>
	{
	public:
		RenderLevel(const std::string& name, std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format);
		virtual ~RenderLevel() = default;

		bool Initialize(ID3D12Device* device);
		void Shutdown();
		void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list);
		
		bool RegisterShader(ID3D12Device* device, const SPtr<Shader>& shader);

	protected:
		virtual bool CreateRootSignature(ID3D12Device* device) = 0;
		virtual void UpdateCommonResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) {}
		virtual void SetRootCommonResource(ID3D12GraphicsCommandList* command_list) {}


	protected:
		std::string m_name;
		ComPtr<ID3D12RootSignature> m_root_signature;

	private:
		std::vector<SPtr<Shader>> m_shaders;
		std::vector<DXGI_FORMAT> m_rtv_formats;
		DXGI_FORMAT m_dsv_format;

	public:
		const std::string& GetName() const { return m_name; }
		const ComPtr<ID3D12RootSignature>& GetRootSignature() const { return m_root_signature; }
		const std::vector<DXGI_FORMAT>& GetRTVFormats() const { return m_rtv_formats; }
		DXGI_FORMAT GetDSVFormat() const { return m_dsv_format; }
	};
}



