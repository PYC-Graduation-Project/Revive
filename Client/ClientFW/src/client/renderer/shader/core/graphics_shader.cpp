#include "stdafx.h"
#include "client/renderer/shader/core/graphics_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	GraphicsShader::GraphicsShader(const std::string& name)
		: Shader(name)
	{
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateShader(ID3DBlob** shader_blob)
	{
		return D3D12_SHADER_BYTECODE{ nullptr, 0 };
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateHullShader(ID3DBlob** shader_blob, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateDomainShader(ID3DBlob** shader_blob, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateGeometryShader(ID3DBlob** shader_blob, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreatePixelShader(ID3DBlob** shader_blob, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_INPUT_LAYOUT_DESC GraphicsShader::CreateInputLayout(int pso_index)
	{
		return D3D12_INPUT_LAYOUT_DESC{ nullptr, 0 };
	}

	D3D12_RASTERIZER_DESC GraphicsShader::CreateRasterizerState(int pso_index)
	{
		return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	}

	D3D12_BLEND_DESC GraphicsShader::CreateBlendState(int pso_index)
	{
		return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	}

	D3D12_DEPTH_STENCIL_DESC GraphicsShader::CreateDepthStencilState(int pso_index)
	{
		return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}

	D3D12_STREAM_OUTPUT_DESC GraphicsShader::CreateStreamOutputState(int pso_index)
	{
		D3D12_STREAM_OUTPUT_DESC desc;
		ZeroMemory(&desc, sizeof(D3D12_STREAM_OUTPUT_DESC));

		desc.NumEntries = 0;
		desc.NumStrides = 0;
		desc.pBufferStrides = nullptr;
		desc.pSODeclaration = nullptr;
		desc.RasterizedStream = 0;

		return desc;
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE GraphicsShader::GetPrimitiveTopologyType(int pso_index)
	{
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}

	bool GraphicsShader::CreatePipelineState(ID3D12Device* device, const SPtr<RenderLevel>& render_level, int num_of_pso)
	{
		std::string render_level_name = render_level->GetName();
		m_pipeline_states[render_level_name].resize(num_of_pso);

		ComPtr<ID3DBlob> vertex_blob, hull_blob, domain_blob, geometry_blob, pixel_blob;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc;
		ZeroMemory(&pso_desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		pso_desc.pRootSignature = render_level->GetRootSignature().Get();
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.NumRenderTargets = static_cast<UINT>(render_level->GetRTVFormats().size());
		for (UINT i = 0; i < pso_desc.NumRenderTargets; ++i)
			pso_desc.RTVFormats[i] = render_level->GetRTVFormats()[i];
		pso_desc.DSVFormat = render_level->GetDSVFormat();
		pso_desc.SampleDesc.Count = D3DUtil::s_is_use_4x_mass ? 4 : 1;
		pso_desc.SampleDesc.Quality = D3DUtil::s_is_use_4x_mass ? D3DUtil::s_4x_msaa_quality - 1 : 0;
		pso_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		for (int i = 0; i < num_of_pso; ++i)
		{
			pso_desc.InputLayout = CreateInputLayout(i);
			pso_desc.VS = CreateVertexShader(vertex_blob.GetAddressOf(), i);
			pso_desc.HS = CreateHullShader(hull_blob.GetAddressOf(), i);
			pso_desc.DS = CreateDomainShader(domain_blob.GetAddressOf(), i);
			pso_desc.GS = CreateGeometryShader(geometry_blob.GetAddressOf(), i);
			pso_desc.PS = CreatePixelShader(pixel_blob.GetAddressOf(), i);
			pso_desc.StreamOutput = CreateStreamOutputState(i);
			pso_desc.RasterizerState = CreateRasterizerState(i);
			pso_desc.BlendState = CreateBlendState(i);
			pso_desc.DepthStencilState = CreateDepthStencilState(i);
			pso_desc.PrimitiveTopologyType = GetPrimitiveTopologyType(i);

			if (FAILED(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pipeline_states[render_level_name][i]))))
			{
				DebugPipelineState(render_level_name, i);
				return false;
			}
		}
		return true;
	}
}
