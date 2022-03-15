#include "stdafx.h"
#include "client/renderer/renderitem/ui_render_item.h"
#include "client/asset/primitive/primitive.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/ui/core/user_interface_layer.h"
#include "client/object/ui/core/user_interface.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	UIRenderItem::UIRenderItem()
	{
		m_ui_primitive = CreateUPtr<UploadPrimitive<UIVertex>>();
	}

	UIRenderItem::~UIRenderItem()
	{
	}

	void UIRenderItem::Initialize(ID3D12Device* device)
	{
		m_ui_primitive->Initialize(device);
	}

	void UIRenderItem::Shutdown()
	{
		m_ui_primitive->Shutdown();
	}

	void UIRenderItem::Update(ID3D12Device* device, 
		ID3D12GraphicsCommandList* command_list, const Vec2& window_size)
	{
		const auto& ui_manager = UserInterfaceManager::GetUIManager();
		const auto& ui_layers = ui_manager.GetUILayers();

		UINT num_of_user_interface_data = 0;

		if (ui_layers.empty())
			m_num_of_draw_ui_data = 0;
		else
		{
			if (ui_layers.size() != m_num_of_ui_layer)
			{
				for (const auto& ui_layer : ui_layers)
				{
					if (ui_layer->GetUILayerState() != eUILayerState::kDead)
						num_of_user_interface_data += ui_layer->GetNumOfVisibleTexture();
				}

				m_ui_primitive->Update(device, num_of_user_interface_data);
			}
		}

		m_num_of_ui_layer = static_cast<UINT>(ui_layers.size());

		if (m_num_of_ui_layer > 0)
		{
			for (const auto& ui_layer : ui_layers)
			{
				if (ui_layer->GetUILayerState() == eUILayerState::kActive)
				{
					const auto& user_interfaces = ui_layer->GetUserInterfaces();

					std::vector<UIVertex> vertices;

					for (const auto& ui : user_interfaces)
					{
						if (ui->IsVisible())
						{
							for (const auto& ui_texture : ui->GetVisibleTextures())
							{
								if (ui_texture == nullptr)
									LOG_WARN("Could not find ui texture : {0}", ui->GetName());
								else
								{
									Vec2 new_position = (ui->GetPosition() + ui_texture->GetPosition()) * Vec2(1.0f, -1.0f) +
										Vec2(window_size.x * -0.5f, window_size.y * 0.5f);
									INT resource_index = -1;

									if (ui_texture->GetTexture() != nullptr)
										resource_index = ui_texture->GetTexture()->GetResourceIndex();

									vertices.emplace_back(UIVertex(new_position, ui_texture->GetSize(), resource_index,
										ui_texture->GetBrushColor(), ui_texture->GetCoordinate(), ui_texture->GetTilling()));
								}
							}
						}
					}

					m_num_of_draw_ui_data = static_cast<UINT>(vertices.size());
					m_ui_primitive->UpdateVertices(vertices);
				}
			}
		}
	}

	void UIRenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		m_ui_primitive->PreDraw(command_list);
		m_ui_primitive->Draw(command_list, m_num_of_draw_ui_data);
	}
}
