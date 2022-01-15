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

	void OpaqueMeshShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const std::string& level_name)
	{
		UpdateRenderItem(device, command_list);
	}

	void OpaqueMeshShader::Draw(ID3D12GraphicsCommandList* command_list, const std::string& level_name)
	{
		switch (HashCode(level_name.c_str()))
		{
		case HashCode("opaque"):
			command_list->SetPipelineState(m_pipeline_states.at(level_name)[0].Get());
			DrawRenderItem(command_list);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE OpaqueMeshShader::CreateVertexShader(ID3DBlob** shader_blob, const std::string& level_name, int pso_index)
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueMesh", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE OpaqueMeshShader::CreatePixelShader(ID3DBlob** shader_blob, const std::string& level_name, int pso_index)
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "PSOpaqueMesh", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> OpaqueMeshShader::CreateInputLayout(const std::string& level_name, int pso_index)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(3);
		input_element_descs.resize(3);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool OpaqueMeshShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (HashCode(render_level->GetName().c_str()))
		{
		case HashCode("opaque"):
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}", render_level->GetName(), m_name);
			return false;
		}

		return result;
	}
}
