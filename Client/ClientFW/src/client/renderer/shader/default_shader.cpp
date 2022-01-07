#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/default_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	DefaultShader::DefaultShader(const std::string& name)
		: GraphicsShader(name)
	{
	}

	void DefaultShader::Initialize(ID3D12Device* device)
	{
	}

	void DefaultShader::Shutdown()
	{
	}

	void DefaultShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const std::string& level_name)
	{
	}

	void DefaultShader::Draw(ID3D12GraphicsCommandList* command_list, const std::string& level_name)
	{
	}

	D3D12_SHADER_BYTECODE DefaultShader::CreateVertexShader(ID3DBlob** shader_blob, int pso_index)
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Default.hlsl", "VSDiffuse", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE DefaultShader::CreatePixelShader(ID3DBlob** shader_blob, int pso_index)
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Default.hlsl", "PSDiffuse", "ps_5_1", shader_blob);
	}

	D3D12_INPUT_LAYOUT_DESC DefaultShader::CreateInputLayout(int pso_index)
	{
		UINT nInputElementDescs = 1;
		D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

		pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
		d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
		d3dInputLayoutDesc.NumElements = nInputElementDescs;

		return d3dInputLayoutDesc;
	}

	bool DefaultShader::CreatePipelineStates(ID3D12Device* device, const SPtr<RenderLevel>& render_level)
	{
		bool result = true;

		switch (HashCode(render_level->GetName().c_str()))
		{
		case HashCode("default"):
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}", render_level->GetName(), m_name);
			return false;
		}

		return result;
	}
}
