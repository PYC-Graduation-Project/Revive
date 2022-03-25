#include "stdafx.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/object/component/light/core/light_component.h"
#include "client/object/component/light/directional_light_component.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	LightManager* LightManager::s_light_manager = nullptr;

	LightManager::LightManager()
	{
		s_light_manager = this;
	}

	LightManager::~LightManager()
	{
	}

	void LightManager::Shutdown()
	{
		s_light_manager = nullptr;
	}

	void LightManager::Update(ID3D12Device* device)
	{
		const auto& light_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetLightFrameResource();
		const auto& light_resource_data = light_resource->GetLightData();

		UINT size_of_light = light_resource->GetSizeOfLight();

		if (size_of_light <= m_num_of_light)
		{
			size_of_light = static_cast<UINT>(roundf(static_cast<float>(size_of_light) * 1.5f));
			light_resource_data->CreateResource(device, size_of_light);
			light_resource->SetSizeOfLight(size_of_light);
		}

		RSLightData light_data;
		UINT index = 0;

		for (const auto& light : m_directional_lights)
		{
			light_data.light_color = light->GetLightColor();
			light_data.light_direction = light->GetDirection();
			light_resource_data->CopyData(index++, light_data);
		}
	}

	void LightManager::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_num_of_light > 0)
		{
			const auto& light_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetLightFrameResource();
			const auto& light_resource_data = light_resource->GetLightData();

			command_list->SetGraphicsRootShaderResourceView(5, light_resource_data->GetResource()->GetGPUVirtualAddress());
		}
	}

	bool LightManager::RegisterLightComponent(const SPtr<LightComponent>& light_comp)
	{
		switch (light_comp->GetLightType())
		{
		case eLightType::kDirectional:
			if (m_directional_lights.size() < 4)
			{
				m_directional_lights.push_back(std::static_pointer_cast<DirectionalLightComponent>(light_comp));
				++m_num_of_light;
			}
			else
				return false;
			break;
		default:
			break;
		}
		return true;
	}

	void LightManager::UnregisterLightComponent(const SPtr<LightComponent>& light_comp)
	{
		switch (light_comp->GetLightType())
		{
		case eLightType::kDirectional:
		{
			auto iter = std::find(m_directional_lights.begin(), m_directional_lights.end(), light_comp);
			if (iter != m_directional_lights.end())
			{
				std::iter_swap(iter, m_directional_lights.end() - 1);
				m_directional_lights.pop_back();
				--m_num_of_light;
			}
			break;
		}
		default:
			break;
		}
	}
}
