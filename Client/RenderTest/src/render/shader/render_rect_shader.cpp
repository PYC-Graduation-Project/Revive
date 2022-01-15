#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include "render/level/render_rect_render_level.h"
#include "render/shader/render_rect_shader.h"

namespace render_test
{
	RenderRectShader::RenderRectShader(const std::string& name)
		: GraphicsShader(name)
	{
	}

	void RenderRectShader::Initialize(ID3D12Device* device)
	{
	}

	void RenderRectShader::Shutdown()
	{
	}

	void RenderRectShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const std::string& level_name)
	{
		UpdateRenderItem(device, command_list);
	}

	void RenderRectShader::Draw(ID3D12GraphicsCommandList* command_list, const std::string& level_name)
	{
		switch (HashCode(level_name.c_str()))
		{
		case HashCode("render rect level"):
			command_list->SetPipelineState(m_pipeline_states.at(level_name)[0].Get());
			DrawRenderItem(command_list);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE RenderRectShader::CreateVertexShader(ID3DBlob** shader_blob, const std::string& level_name, int pso_index)
	{
		return CompileShader(L"../RenderTest/src/render/hlsl/render_rect.hlsl", "VSDiffuse", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE RenderRectShader::CreatePixelShader(ID3DBlob** shader_blob, const std::string& level_name, int pso_index)
	{
		return CompileShader(L"../RenderTest/src/render/hlsl/render_rect.hlsl", "PSDiffuse", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> RenderRectShader::CreateInputLayout(const std::string& level_name, int pso_index)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);
		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool RenderRectShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (HashCode(render_level->GetName().c_str()))
		{
		case HashCode("render rect level"):
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}", render_level->GetName(), m_name);
			return false;
		}

		return result;
	}
}
