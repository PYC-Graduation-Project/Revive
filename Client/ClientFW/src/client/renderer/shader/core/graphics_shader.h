#pragma once
#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	class GraphicsShader : public Shader
	{
	public:
		GraphicsShader(const std::string& name);
		virtual ~GraphicsShader() = default;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, int pso_index) = 0;
		virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob** shader_blob, int pso_index);
		virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** shader_blob, int pso_index);
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, int pso_index);
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, int pso_index);

		virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout(int pso_index);
		virtual D3D12_RASTERIZER_DESC CreateRasterizerState(int pso_index);
		virtual D3D12_BLEND_DESC CreateBlendState(int pso_index);
		virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(int pso_index);
		virtual D3D12_STREAM_OUTPUT_DESC CreateStreamOutputState(int pso_index);

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(int pso_index);

	protected:
		virtual bool CreatePipelineState(ID3D12Device* device, const SPtr<RenderLevel>& render_level, int num_of_pso) override final;

	private:
		virtual D3D12_SHADER_BYTECODE CreateShader(ID3DBlob** shader_blob);
	};
}


