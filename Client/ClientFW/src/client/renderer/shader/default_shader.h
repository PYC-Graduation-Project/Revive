#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class DefaultShader final : public GraphicsShader
	{
	public:
		DefaultShader(const std::string& name);
		virtual ~DefaultShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const std::string& level_name) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, const std::string& level_name) override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, int pso_index) override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, int pso_index) override;

		virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout(int pso_index) override;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<RenderLevel>& render_level) override;
	};
}


