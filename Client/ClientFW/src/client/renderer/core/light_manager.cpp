#include "stdafx.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/object/component/light/core/light_component.h"
#include "client/object/component/light/directional_light_component.h"
#include "client/object/component/light/point_light_component.h"
#include "client/object/component/light/spot_light_component.h"
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

		UINT index = 0;

		for (const auto& light : m_directional_lights)
		{
			RSLightData light_data;
			light_data.light_color = light->GetLightColor();
			light_data.light_direction = light->GetDirection();
			light->SetLightManagerRegisteredIndex(index);
			light_resource_data->CopyData(index++, light_data);
		}

		for (const auto& light : m_point_lights)
		{
			RSLightData light_data;
			light_data.light_color = light->GetLightColor();
			light_data.light_position = light->GetWorldPosition();
			light_data.attenuation_radius = light->GetAttenuationRadius();
			light->SetLightManagerRegisteredIndex(index);
			light_resource_data->CopyData(index++, light_data);
		}

		for (const auto& light : m_spot_lights)
		{
			RSLightData light_data;
			light_data.light_color = light->GetLightColor();
			light_data.light_position = light->GetWorldPosition();
			light_data.light_direction = light->GetWorldForward();
			light_data.attenuation_radius = light->GetAttenuationRadius();
			light_data.cone_inner_angle = light->GetConeInnerAngle();
			light_data.cone_outer_angle = light->GetConeOuterAngle();
			light->SetLightManagerRegisteredIndex(index);
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
		{
			if (m_directional_lights.size() < 4)
				RegisterLightComponent(m_directional_lights, light_comp);
			else
				return false;
			break;
		}
		case eLightType::kPoint:
		{
			RegisterLightComponent(m_point_lights, light_comp);
			break;
		}
		case eLightType::kSpot:
		{
			RegisterLightComponent(m_spot_lights, light_comp);
			break;
		}
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
			UnregisterLightComponent(m_directional_lights, light_comp);
			break;
		}
		case eLightType::kPoint:
		{
			UnregisterLightComponent(m_point_lights, light_comp);
			break;
		}
		case eLightType::kSpot:
		{
			UnregisterLightComponent(m_spot_lights, light_comp);
			break;
		}
		default:
			break;
		}
	}
}
