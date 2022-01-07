#include "stdafx.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	RenderLevel::RenderLevel(const std::string& name,
		std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format)
		: m_name(name), m_rtv_formats(rtv_formats), m_dsv_format(dsv_format)
	{
	}

	bool RenderLevel::Initialize(ID3D12Device* device)
	{
		if (CreateRootSignature(device) == false)
		{
			LOG_ERROR("Could not create root signature [{0}]", m_name);
			return false;
		}

		return true;
	}

	void RenderLevel::Shutdown()
	{
		m_shaders.clear();
	}

	void RenderLevel::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		UpdateCommonResource(device, command_list);

		for (const auto& shader : m_shaders)
			shader->Update(device, command_list, m_name);
	}

	void RenderLevel::Draw(ID3D12GraphicsCommandList* command_list)
	{
		command_list->SetGraphicsRootSignature(m_root_signature.Get());
		SetRootCommonResource(command_list);

		for (const auto& shader : m_shaders)
			shader->Draw(command_list, m_name);
	}

	bool RenderLevel::RegisterShader(ID3D12Device* device, const SPtr<Shader>& shader)
	{
		m_shaders.push_back(shader);
		return shader->CreatePipelineStates(device, shared_from_this());
	}
}
