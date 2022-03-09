#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/renderer/renderlevel/core/render_level.h>
#include <client/renderer/core/render.h>
#include "render/shader/render_anim_shader.h"

namespace anim_test
{
	RenderAnimShader::RenderAnimShader(const std::string& name)
		: MeshShader(name)
	{
	}

	void RenderAnimShader::Initialize(ID3D12Device* device)
	{
	}

	void RenderAnimShader::Shutdown()
	{
	}

	void RenderAnimShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type)
	{
		UpdateRenderItem(device, command_list);
	}

	void RenderAnimShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			DrawRenderItem(command_list);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE RenderAnimShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../AnimationTest/src/render/hlsl/render_anim.hlsl", "VSSkinnedMesh", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE RenderAnimShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../AnimationTest/src/render/hlsl/render_anim.hlsl", "PSSkinnedMesh", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> RenderAnimShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);
		input_element_descs.resize(5);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[3] = { "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[4] = { "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool RenderAnimShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
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
