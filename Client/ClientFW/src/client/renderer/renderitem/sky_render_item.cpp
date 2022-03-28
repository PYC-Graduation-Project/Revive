#include "stdafx.h"
#include "client/renderer/renderitem/sky_render_item.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/sky_frame_resource.h"
#include "client/asset/mesh/cube_mesh.h"
#include "client/asset/texture/texture.h"
#include "client/object/component/sky/sky_cube_component.h"

namespace client_fw
{
	SkyRenderItem::SkyRenderItem(const std::string& owner_shader_name)
		: m_owner_shader_name(owner_shader_name)
	{
		m_sky_cube_mesh = CreateSPtr<SkyCubeMesh>();
	}

	SkyRenderItem::~SkyRenderItem()
	{
	}

	void SkyRenderItem::Initialize(ID3D12Device* device)
	{
		RenderResourceManager::GetRenderResourceManager().RegisterPrimitive(m_sky_cube_mesh);
		const auto& frame_resource = FrameResourceManager::GetManager().GetFrameResources();
		for (const auto& frame : frame_resource)
			frame->CreateSkyFrameResource(device, m_owner_shader_name);
	}

	void SkyRenderItem::Shutdown()
	{
	}

	void SkyRenderItem::Update(ID3D12Device* device)
	{
		if (m_sky_components.empty() == false)
		{
			if (m_is_need_update_draw_sky_component)
			{
				const auto& last_sky = *m_sky_components.rbegin();
				
				if (last_sky->GetSkyType() == eSkyType::kCube)
				{
					m_draw_sky_type = eSkyType::kCube;
					m_draw_sky_cube_component = std::static_pointer_cast<SkyCubeComponent>(last_sky);
				}
			}
		}
		else
		{
			m_draw_sky_cube_component = nullptr;
		}
		
	}

	void SkyRenderItem::UpdateFrameResource(ID3D12Device* device)
	{
		if (m_sky_components.empty() == false)
		{
			if (m_is_need_update_draw_sky_component && 
				m_draw_sky_cube_component != nullptr)
			{
				const auto& sky_frame_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetSkyFrameResource(m_owner_shader_name);

				if (m_draw_sky_type == eSkyType::kCube)
				{
					UINT index = m_draw_sky_cube_component->GetCubeMapTexture()->GetResourceIndex();
					sky_frame_resource->GetCubeTextureIndexData()->CopyData(0, 
						RSCubeTextureIndexData{ index });
				}

				m_is_need_update_draw_sky_component = false;
			}
		}
	}

	void SkyRenderItem::Draw(ID3D12GraphicsCommandList* command_list, std::function<void()>&& draw_function)
	{
		if (m_sky_components.empty() == false)
		{
			const auto& sky_frame_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetSkyFrameResource(m_owner_shader_name);
			command_list->SetGraphicsRootConstantBufferView(0, sky_frame_resource->GetCubeTextureIndexData()->GetResource()->GetGPUVirtualAddress());

			draw_function();
			m_sky_cube_mesh->Draw(command_list);
		}
	}

	void SkyRenderItem::RegisterSkyComponent(const SPtr<SkyComponent>& sky_component)
	{
		m_sky_components.push_back(sky_component);
		m_is_need_update_draw_sky_component = true;
	}

	void SkyRenderItem::UnregisterSkyComponent(const SPtr<SkyComponent>& sky_component)
	{
		auto iter = std::find(m_sky_components.begin(), m_sky_components.end(), sky_component);
		if (iter != m_sky_components.end())
		{
			std::iter_swap(iter, m_sky_components.end() - 1);
			m_sky_components.pop_back();
			m_is_need_update_draw_sky_component = true;
		}
	}
}
