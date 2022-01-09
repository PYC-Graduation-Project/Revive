#pragma once

namespace client_fw
{
	class Shader
	{
	public:
		Shader(const std::string& name);
		virtual ~Shader() = default;

		virtual void Initialize(ID3D12Device* device) = 0;
		virtual void Shutdown() = 0;
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const std::string& level_name) = 0;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, const std::string& level_name) = 0;

		D3D12_SHADER_BYTECODE CompileShader(std::wstring_view file_name,
			std::string_view entry_point, std::string_view version, ID3DBlob** shader_blob);

	protected:
		void DebugPipelineState(const std::string& render_level_name, int index);

	protected:
		std::string m_name;
		std::map<std::string, std::vector<ComPtr<ID3D12PipelineState>>> m_pipeline_states;

	public:
		const std::string& GetName() const { return m_name; }
	};
}



