#include "stdafx.h"
#include "client/renderer/shader/material_billboard_shader.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/renderitem/billboard_render_item.h"

namespace client_fw
{
	MaterialBillboardShader::MaterialBillboardShader(const std::string& name)
		: BillboardShader(name)
	{
		m_billboard_render_item = CreateSPtr<MaterialBillboardRenderItem>();
	}

	void MaterialBillboardShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			UpdateRenderItem(device);
			break;
		default:
			break;
		}
	}

	void MaterialBillboardShader::UpdateFrameResource(ID3D12Device* device)
	{
		UpdateRenderItemResource(device);
	}

	void MaterialBillboardShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			DrawRenderItem(command_list, [this, command_list, level_type]() {
				command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				},
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[1].Get());
				});
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE MaterialBillboardShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "VSMaterialBillboard", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE MaterialBillboardShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if(pso_index ==0)
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSMaterialBillboard", "gs_5_1", shader_blob);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSFixUpMaterialBillboard", "gs_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> MaterialBillboardShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(3);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "RSINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool MaterialBillboardShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
			result &= CreatePipelineState(device, render_level, 2);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	OpaqueMaterialBillboardShader::OpaqueMaterialBillboardShader(const std::string& name)
		: MaterialBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueMaterialBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSOpaqueMaterialBillboard", "ps_5_1", shader_blob);
	}
}
