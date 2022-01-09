#pragma once
#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	class GraphicsRenderLevel;
	class MeshComponent;
	class RenderItem;

	class GraphicsShader : public Shader
	{
	public:
		GraphicsShader(const std::string& name);
		virtual ~GraphicsShader() = default;

		void UpdateRenderItem(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void DrawRenderItem(ID3D12GraphicsCommandList* command_list);

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

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) = 0;

	protected:
		bool CreatePipelineState(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level, int num_of_pso);

	private:
		virtual D3D12_SHADER_BYTECODE CreateShader(ID3DBlob** shader_blob);

	public:
		virtual bool RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);

	private:
		std::vector<SPtr<RenderItem>> m_render_items;
		std::unordered_map<std::string, SPtr<RenderItem>> m_render_items_map;
	};
}


