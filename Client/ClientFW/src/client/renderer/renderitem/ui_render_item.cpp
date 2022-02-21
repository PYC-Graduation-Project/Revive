#include "stdafx.h"
#include "client/renderer/renderitem/ui_render_item.h"
#include "client/asset/primitive/ui_primitive.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/ui/core/user_interface.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	UIRenderItem::UIRenderItem()
	{
		m_ui_primitive = CreateUPtr<UIPrimitive>();
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

	void UIRenderItem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
		const SPtr<CameraComponent> main_camera)
	{
		const auto& ui_manager = UserInterfaceManager::GetUIManager();
		if (ui_manager != nullptr)
		{
			const auto& ui_interfaces = ui_manager->GetUserInterfaces();

			UINT num_of_user_interface = static_cast<UINT>(ui_interfaces.size());

			if (num_of_user_interface > 0)
			{
				if (m_size_of_user_interface_data == 0)
				{
					m_size_of_user_interface_data = 1;
					m_ui_primitive->Update(device, m_size_of_user_interface_data);
				}
				else if (m_size_of_user_interface_data < num_of_user_interface)
				{
					m_size_of_user_interface_data = static_cast<UINT>(roundf(static_cast<float>(m_size_of_user_interface_data) * 1.5f));
					m_ui_primitive->Update(device, m_size_of_user_interface_data);
				}

				std::vector<UIVertex> vertices;
				
				for (const auto& ui : ui_interfaces)
				{
					if (ui->GetUIState() == eUIState::kActive && ui->GetTexture() != nullptr)
					{
						Vec2 new_position = ui->GetPosition() * Vec2(1.0f, -1.0f) + 
							Vec2(-main_camera->GetViewport().width * 0.5f, main_camera->GetViewport().height * 0.5f);
						vertices.emplace_back(UIVertex(new_position, ui->GetSize(), ui->GetTexture()->GetResourceIndex()));
					}
				}

				m_num_of_draw_ui_data = static_cast<UINT>(vertices.size());
				m_ui_primitive->UpdateUIVertices(vertices);
			}

		}
		else
		{
			m_size_of_user_interface_data = 0;
			m_ui_primitive->Shutdown();
		}
	}

	void UIRenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_num_of_draw_ui_data > 0)
			m_ui_primitive->Draw(command_list, m_num_of_draw_ui_data);
	}
}
