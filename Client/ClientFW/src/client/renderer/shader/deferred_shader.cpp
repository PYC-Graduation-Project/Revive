#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/deferred_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	DeferredShader::DeferredShader(const std::string& name)
		: GraphicsShader(name)
	{
	}

	void DeferredShader::Initialize(ID3D12Device* device)
	{
	}

	void DeferredShader::Shutdown()
	{
	}

	void DeferredShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
	}

	void DeferredShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kDeferred:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			command_list->DrawInstanced(6, 1, 0, 0);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE DeferredShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/UI.hlsl", "VSRenderTexture", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE DeferredShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "PSRenderTexture", "ps_5_1", shader_blob);
	}

	D3D12_DEPTH_STENCIL_DESC DeferredShader::CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_DEPTH_STENCIL_DESC desc = GraphicsShader::CreateDepthStencilState(level_type, pso_index);
		desc.DepthEnable = FALSE;
		return desc;
	}

	bool DeferredShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kDeferred:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}
}
