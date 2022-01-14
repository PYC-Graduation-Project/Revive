#include "stdafx.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/shader/core/shader.h"
#include "client/renderer/shader/core/graphics_shader.h"
#include "client/object/component/util/camera_component.h"

namespace client_fw
{
	RenderLevel::RenderLevel(const std::string& name)
		: m_name(name)
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

	GraphicsRenderLevel::GraphicsRenderLevel(const std::string& name,
		std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format)
		: RenderLevel(name), m_rtv_formats(rtv_formats), m_dsv_format(dsv_format)
	{
	}

	bool GraphicsRenderLevel::Initialize(ID3D12Device* device)
	{
		return RenderLevel::Initialize(device);
	}

	void GraphicsRenderLevel::Shutdown()
	{
		m_graphics_shaders.clear();
	}

	void GraphicsRenderLevel::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		UpdateCommonResource(device, command_list);

		for (const auto& shader : m_graphics_shaders)
			shader->Update(device, command_list, m_name);
	}

	void GraphicsRenderLevel::Draw(ID3D12GraphicsCommandList* command_list, const std::vector<SPtr<CameraComponent>>& cameras)
	{
		command_list->SetGraphicsRootSignature(m_root_signature.Get());
		SetRootCommonResource(command_list);

		if (cameras.empty())
		{
			for (const auto& shader : m_graphics_shaders)
				shader->Draw(command_list, m_name);
		}
		else
		{
			for (const auto& camera : cameras)
			{
				if (camera->GetCameraState() == eCameraState::kActive)
				{
					SetRootCameraResource(command_list, camera);
					for (const auto& shader : m_graphics_shaders)
						shader->Draw(command_list, m_name);
				}
			}
		}
	}

	bool GraphicsRenderLevel::RegisterGraphicsShader(ID3D12Device* device, const SPtr<GraphicsShader>& graphics_shader)
	{
		m_graphics_shaders.push_back(graphics_shader);
		return graphics_shader->CreatePipelineStates(device, shared_from_this());
	}

	void GraphicsRenderLevel::UnregisterGraphicsShader(const SPtr<GraphicsShader>& graphics_shader)
	{
		auto iter = std::find(m_graphics_shaders.begin(), m_graphics_shaders.end(), graphics_shader);
		if (iter != m_graphics_shaders.cend())
		{
			std::iter_swap(iter, m_graphics_shaders.end() - 1);
			m_graphics_shaders.pop_back();
		}
	}
}