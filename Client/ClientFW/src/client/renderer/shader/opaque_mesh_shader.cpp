#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/opaque_mesh_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	OpaqueMeshShader::OpaqueMeshShader(const std::string& name)
		: GraphicsShader(name)
	{
	}

	void OpaqueMeshShader::Initialize(ID3D12Device* device)
	{
	}

	void OpaqueMeshShader::Shutdown()
	{
	}

	void OpaqueMeshShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type)
	{
		UpdateRenderItem(device, command_list);
	}

	void OpaqueMeshShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type)
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

	D3D12_SHADER_BYTECODE OpaqueMeshShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index)
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueMesh", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE OpaqueMeshShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index)
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "PSOpaqueMesh", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> OpaqueMeshShader::CreateInputLayout(eRenderLevelType level_type, int pso_index)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(3);
		input_element_descs.resize(3);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool OpaqueMeshShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
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
