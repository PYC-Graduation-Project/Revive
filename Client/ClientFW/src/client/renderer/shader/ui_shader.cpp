#include "stdafx.h"
#include "client/renderer/shader/ui_shader.h"
#include "client/renderer/renderitem/ui_render_item.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/core/camera_manager.h"

namespace client_fw
{
	UIShader::UIShader(const std::string& name)
		: GraphicsShader(name)
	{
		m_render_item = CreateSPtr<UIRenderItem>();
	}

	void UIShader::Initialize(ID3D12Device* device)
	{
		m_render_item->Initialize(device);
	}

	void UIShader::Shutdown()
	{
		m_render_item->Shutdown();
	}

	void UIShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case eRenderLevelType::kUI:
			m_render_item->Update(device, command_list, CameraManager::GetCameraManager().GetMainCamera());
			break;
		default:
			break;
		}
	}

	void UIShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kUI:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			m_render_item->Draw(command_list);
			break;
		default:
			break;
		}
	}
	D3D12_SHADER_BYTECODE UIShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/UI.hlsl", "VSRenderUI", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE UIShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/UI.hlsl", "GSRenderUI", "gs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE UIShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/UI.hlsl", "PSRenderUI", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> UIShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(5);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "TEXINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[3] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[4] = { "TILLING", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	D3D12_BLEND_DESC UIShader::CreateBlendState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_BLEND_DESC desc = GraphicsShader::CreateBlendState(level_type, pso_index);
		desc.AlphaToCoverageEnable = TRUE;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		return desc;
	}

	D3D12_DEPTH_STENCIL_DESC UIShader::CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_DEPTH_STENCIL_DESC desc = GraphicsShader::CreateDepthStencilState(level_type, pso_index);
		desc.DepthEnable = FALSE;
		return desc;
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE UIShader::GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const
	{
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}

	bool UIShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kUI:
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
