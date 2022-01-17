#include "stdafx.h"
#include "client/renderer/shader/core/graphics_shader.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/core/render_item.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/asset/mesh/mesh.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	GraphicsShader::GraphicsShader(const std::string& name)
		: Shader(name)
	{
	}

	void GraphicsShader::UpdateRenderItem(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& render_item : m_render_items)
			render_item->Update(device, command_list);
	}

	void GraphicsShader::DrawRenderItem(ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& render_item : m_render_items)
			render_item->Draw(command_list);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateShader(ID3DBlob** shader_blob)
	{
		return D3D12_SHADER_BYTECODE{ nullptr, 0 };
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	D3D12_SHADER_BYTECODE GraphicsShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index)
	{
		return CreateShader(shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> GraphicsShader::CreateInputLayout(eRenderLevelType level_type, int pso_index)
	{
		return std::vector<D3D12_INPUT_ELEMENT_DESC>();
	}

	D3D12_RASTERIZER_DESC GraphicsShader::CreateRasterizerState(eRenderLevelType level_type, int pso_index)
	{
		return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	}

	D3D12_BLEND_DESC GraphicsShader::CreateBlendState(eRenderLevelType level_type, int pso_index)
	{
		return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	}

	D3D12_DEPTH_STENCIL_DESC GraphicsShader::CreateDepthStencilState(eRenderLevelType level_type, int pso_index)
	{
		return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}

	D3D12_STREAM_OUTPUT_DESC GraphicsShader::CreateStreamOutputState(eRenderLevelType level_type, int pso_index)
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

	D3D12_PRIMITIVE_TOPOLOGY_TYPE GraphicsShader::GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index)
	{
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}

	bool GraphicsShader::CreatePipelineState(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level, int num_of_pso)
	{
		eRenderLevelType render_level_type = render_level->GetRenderLevelType();
		m_pipeline_states[render_level_type].resize(num_of_pso);

		ComPtr<ID3DBlob> vertex_blob, hull_blob, domain_blob, geometry_blob, pixel_blob;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc;
		ZeroMemory(&pso_desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		pso_desc.pRootSignature = render_level->GetRootSignature()->GetRootSignature();
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
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs = CreateInputLayout(render_level_type, i);
			pso_desc.InputLayout = { input_element_descs.data(), static_cast<UINT>(input_element_descs.size()) };
			pso_desc.VS = CreateVertexShader(vertex_blob.GetAddressOf(), render_level_type, i);
			pso_desc.HS = CreateHullShader(hull_blob.GetAddressOf(), render_level_type, i);
			pso_desc.DS = CreateDomainShader(domain_blob.GetAddressOf(), render_level_type, i);
			pso_desc.GS = CreateGeometryShader(geometry_blob.GetAddressOf(), render_level_type, i);
			pso_desc.PS = CreatePixelShader(pixel_blob.GetAddressOf(), render_level_type, i);
			pso_desc.StreamOutput = CreateStreamOutputState(render_level_type, i);
			pso_desc.RasterizerState = CreateRasterizerState(render_level_type, i);
			pso_desc.BlendState = CreateBlendState(render_level_type, i);
			pso_desc.DepthStencilState = CreateDepthStencilState(render_level_type, i);
			pso_desc.PrimitiveTopologyType = GetPrimitiveTopologyType(render_level_type, i);

			if (FAILED(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pipeline_states[render_level_type][i]))))
			{
				DebugPipelineState(render_level_type, i);
				return false;
			}
		}
		return true;
	}

	bool GraphicsShader::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		std::string path = mesh_comp->GetMesh()->GetPath();

		if (m_render_items_map.find(path) != m_render_items_map.cend())
		{
			m_render_items_map[path]->RegisterMeshComponent(mesh_comp);
		}
		else
		{
			SPtr<RenderItem> render_item = CreateSPtr<RenderItem>(mesh_comp->GetMesh(), 1);	 //수정 필요
			render_item->RegisterMeshComponent(mesh_comp);
			m_render_items_map.insert({ path, render_item });
			m_render_items.push_back(render_item);
		}

		return true;
	}

	void GraphicsShader::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp)
	{
		std::string path = mesh_comp->GetMesh()->GetPath();

		if (m_render_items_map.find(path) != m_render_items_map.cend())
		{
			m_render_items_map[path]->UnregisterMeshComponent(mesh_comp);
		}
	}

}
