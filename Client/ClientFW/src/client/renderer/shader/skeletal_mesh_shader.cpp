#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/skeletal_mesh_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/renderitem/mesh_render_item.h"

namespace client_fw
{
	SkeletalMeshShader::SkeletalMeshShader(const std::string& name)
		: MeshShader(name)
	{
		m_render_item = CreateSPtr<SkeletalMeshRenderItem>(name);
	}

	void SkeletalMeshShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			m_render_item->Update(device, level_type);
			break;
		default:
			break;
		}
	}

	void SkeletalMeshShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_render_item->UpdateFrameResource(device, level_type);
	}

	void SkeletalMeshShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			m_render_item->Draw(command_list, level_type);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE SkeletalMeshShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Skeletal.hlsl", "VSSkeletalMesh", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE SkeletalMeshShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Skeletal.hlsl", "PSSkeletalMesh", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> SkeletalMeshShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);
		input_element_descs.resize(5);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[3] = { "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[4] = { "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool SkeletalMeshShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
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
