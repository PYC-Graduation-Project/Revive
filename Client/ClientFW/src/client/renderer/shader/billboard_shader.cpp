#include "stdafx.h"
#include "client/renderer/shader/billboard_shader.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/renderitem/billboard_render_item.h"

namespace client_fw
{
	BillboardShader::BillboardShader(const std::string& name)
		: GraphicsShader(name)
	{
		m_billboard_render_item = CreateSPtr<BillboardRenderItem>();
	}

	void BillboardShader::Initialize(ID3D12Device* device)
	{
		m_billboard_render_item->Initialize(device);
	}

	void BillboardShader::Shutdown()
	{
		m_billboard_render_item->Shutdown();
	}

	void BillboardShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			m_billboard_render_item->Update(device, command_list);
			break;
		default:
			break;
		}
	}

	void BillboardShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			m_billboard_render_item->PreDraw(command_list);
			if (m_billboard_render_item->IsDrawDataEmpty(eBillboardRenderType::kTexture) == false)
			{
				command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				m_billboard_render_item->Draw(command_list, eBillboardRenderType::kTexture);
			}
			if (m_billboard_render_item->IsDrawDataEmpty(eBillboardRenderType::kFixUpTexture) == false)
			{
				command_list->SetPipelineState(m_pipeline_states.at(level_type)[1].Get());
				m_billboard_render_item->Draw(command_list, eBillboardRenderType::kFixUpTexture);
			}
			if (m_billboard_render_item->IsDrawDataEmpty(eBillboardRenderType::kMaterial) == false)
			{
				command_list->SetPipelineState(m_pipeline_states.at(level_type)[2].Get());
				m_billboard_render_item->Draw(command_list, eBillboardRenderType::kMaterial);
			}
			if (m_billboard_render_item->IsDrawDataEmpty(eBillboardRenderType::kFixUpMaterial) == false)
			{
				command_list->SetPipelineState(m_pipeline_states.at(level_type)[3].Get());
				m_billboard_render_item->Draw(command_list, eBillboardRenderType::kFixUpMaterial);
			}
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE BillboardShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 0:
		case 1:
		default:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "VSBillboard", "vs_5_1", shader_blob);
		case 2:
		case 3:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "VSMaterialBillboard", "vs_5_1", shader_blob);
		}
	
	}

	D3D12_SHADER_BYTECODE BillboardShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 0:
		default:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSBillboard", "gs_5_1", shader_blob);
		case 1:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSFixUpBillboard", "gs_5_1", shader_blob);
		case 2:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSMaterialBillboard", "gs_5_1", shader_blob);
		case 3:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSFixUpMaterialBillboard", "gs_5_1", shader_blob);
		}
	}

	D3D12_SHADER_BYTECODE BillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 0:
		case 1:
		default:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSBillboard", "ps_5_1", shader_blob);
		case 2:
		case 3:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSMaterialBillboard", "ps_5_1", shader_blob);
		}
		
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> BillboardShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 0:
		case 1:
		default:
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(4);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[2] = { "RSINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[3] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		case 2:
		case 3:
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(3);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[2] = { "RSINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		}
	}

	D3D12_BLEND_DESC BillboardShader::CreateBlendState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_BLEND_DESC desc = GraphicsShader::CreateBlendState(level_type, pso_index);
		desc.AlphaToCoverageEnable = TRUE;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		return desc;
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE BillboardShader::GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const
	{
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}

	bool BillboardShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
			result &= CreatePipelineState(device, render_level, 4);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	bool BillboardShader::RegisterBillboardComponent(ID3D12Device* device, const SPtr<BillboardComponent>& bb_comp)
	{
		m_billboard_render_item->RegisterBillboardComponent(bb_comp);
		return true;
	}

	void BillboardShader::UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		m_billboard_render_item->UnregisterBillboardComponent(bb_comp);
	}
}
