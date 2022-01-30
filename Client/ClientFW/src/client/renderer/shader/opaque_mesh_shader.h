#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class OpaqueMeshShader final : public GraphicsShader
	{
	public:
		OpaqueMeshShader(const std::string& name);
		virtual ~OpaqueMeshShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;
		virtual void Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) override;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;
	};
}


