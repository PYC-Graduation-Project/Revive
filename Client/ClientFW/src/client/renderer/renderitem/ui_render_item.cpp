#include "stdafx.h"
#include "client/renderer/renderitem/ui_render_item.h"
#include "client/asset/primitive/ui_primitive.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/ui/core/user_interface.h"
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

	void UIRenderItem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		const auto& ui_manager = UserInterfaceManager::GetUIManager();
		if (ui_manager != nullptr)
		{
			const auto& ui_interfaces = ui_manager->GetUserInterfaces();

			UINT num_of_user_interface = static_cast<UINT>(ui_interfaces.size());
			if (m_num_of_user_interface < num_of_user_interface)
			{
				m_ui_primitive->Update(device, num_of_user_interface);
				m_num_of_user_interface = num_of_user_interface;
			}

			if (num_of_user_interface > 0)
			{
				std::vector<UIVertex> vertices;

				UINT index = 0;
				for (const auto& ui : ui_interfaces)
				{
					if (ui->GetUIState() == eUIState::kActive && ui->GetTexture() != nullptr)
						vertices.emplace_back(UIVertex(ui->GetPosition(), ui->GetSize(), ui->GetTexture()->GetResourceIndex()));
					else
						vertices.emplace_back(UIVertex(vec2::ZERO, vec2::ZERO, 0));
					++index;
				}
				
				for (index; index < num_of_user_interface; ++index)
					vertices.emplace_back(UIVertex(vec2::ZERO, vec2::ZERO, 0));

				m_ui_primitive->UpdateUIVertices(vertices);

			}
		}
		else
		{
			m_num_of_user_interface = 0;
			m_ui_primitive->Shutdown();
		}
	}

	void UIRenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_num_of_user_interface > 0)
			m_ui_primitive->Draw(command_list);
	}
}
