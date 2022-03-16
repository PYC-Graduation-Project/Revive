#include "stdafx.h"
#include "client/renderer/renderitem/widget_render_item.h"
#include "client/asset/primitive/primitive.h"
#include "client/asset/texture/texture.h"
#include "client/object/component/render/widget_component.h"
#include "client/object/ui/core/user_interface_layer.h"
#include "client/object/ui/core/user_interface.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	WidgetRenderItem::WidgetRenderItem()
	{
		m_world_widget_primitive = CreateUPtr<UploadPrimitive<WorldWidgetVertex>>();
		m_widget_primitive = CreateUPtr<UploadPrimitive<PivotWidgetVertex>>();
	}

	WidgetRenderItem::~WidgetRenderItem()
	{
	}

	void WidgetRenderItem::Initialize(ID3D12Device* device)
	{
		m_world_widget_primitive->Initialize(device);
		m_widget_primitive->Initialize(device);
	}

	void WidgetRenderItem::Shutdown()
	{
		m_world_widget_primitive->Shutdown();
		m_widget_primitive->Shutdown();
	}

	void WidgetRenderItem::Update(ID3D12Device* device)
	{
		UpdateWorldWidgets(device);
		UpdateWidgets(device);		
	}

	void WidgetRenderItem::UpdateWorldWidgets(ID3D12Device* device)
	{
		if (m_is_need_world_widget_resource_create)
		{
			if (m_world_widget_components.empty())
			{
				m_world_widget_primitive->Shutdown();
				m_num_of_draw_world_widget_ui_data = 0;
			}
			else
				m_world_widget_primitive->Update(device, m_num_of_world_widget_ui_data);
			m_is_need_world_widget_resource_create = false;
		}

		if (m_real_num_of_world_widget_ui_data > 0)
		{
			std::vector<WorldWidgetVertex> vertices;

			for (const auto& widget : m_world_widget_components)
			{
				Vec3 position = widget->GetWorldPosition();
				const Vec3& right = widget->GetWorldRight();
				const Vec3& up = widget->GetWorldUp();

				const Vec2& widget_size = widget->GetSize();
				const Vec2& move_pos = widget->GetPivot() * widget_size;
				position += ((up * move_pos.y) - (right * move_pos.x));

				if (widget->IsVisible())
				{
					for (const auto& ui : widget->GetUserInterfaceLayer()->GetUserInterfaces())
					{
						const Vec2& lt = ui->GetPosition();
						if (ui->IsVisible() &&
							((lt.x < 0) || (lt.y < 0) || (lt.x > widget_size.x) || (lt.y > widget_size.y)) == false)
						{
							for (const auto& ui_texture : ui->GetVisibleTextures())
							{
								if (ui_texture == nullptr)
									LOG_WARN("Could not find ui texture : {0}", ui->GetName());
								else
								{
									Vec3 new_position = position + right * lt.x + up * lt.y;

									INT resource_index = -1;

									if (ui_texture->GetTexture() != nullptr)
										resource_index = ui_texture->GetTexture()->GetResourceIndex();

									vertices.emplace_back(WorldWidgetVertex(new_position, ui_texture->GetSize(), resource_index,
										ui_texture->GetBrushColor(), ui_texture->GetCoordinate(), ui_texture->GetTilling(),
										right, up));
								}
							}
						}
					}

					widget->SetVisiblity(false);
				}
			}

			m_num_of_draw_world_widget_ui_data = static_cast<UINT>(vertices.size());
			m_world_widget_primitive->UpdateVertices(vertices);
		}
	}

	void WidgetRenderItem::UpdateWidgets(ID3D12Device* device)
	{
		if (m_is_need_widget_resource_create)
		{
			if (m_widget_components.empty())
			{
				m_widget_primitive->Shutdown();
				m_num_of_draw_widget_ui_data = { 0, 0, 0 };
			}
			else
				m_widget_primitive->Update(device, m_num_of_widget_ui_data);
			m_is_need_widget_resource_create = false;
		}

		if (m_real_num_of_widget_ui_data > 0)
		{
			std::vector<PivotWidgetVertex> vertices;
			std::vector<PivotWidgetVertex> fix_up_vertices;

			for (const auto& widget : m_widget_components)
			{
				Vec3 position = widget->GetWorldPosition();
				const Vec3& right = widget->GetWorldRight();
				const Vec3& up = widget->GetWorldUp();

				const Vec2& widget_size = widget->GetSize();
				const Vec2& pivot = widget->GetPivot() * widget_size;

				if (widget->IsVisible())
				{
					for (const auto& ui : widget->GetUserInterfaceLayer()->GetUserInterfaces())
					{
						const Vec2& lt = ui->GetPosition();
						if (ui->IsVisible() &&
							((lt.x < 0) || (lt.y < 0) || (lt.x > widget_size.x) || (lt.y > widget_size.y)) == false)
						{
							for (const auto& ui_texture : ui->GetVisibleTextures())
							{
								if (ui_texture == nullptr)
									LOG_WARN("Could not find ui texture : {0}", ui->GetName());
								else
								{
									Vec3 new_position = position + right * lt.x + up * lt.y;

									INT resource_index = -1;

									if (ui_texture->GetTexture() != nullptr)
										resource_index = ui_texture->GetTexture()->GetResourceIndex();

									PivotWidgetVertex vertex(PivotWidgetVertex(new_position, ui_texture->GetSize(), resource_index,
										ui_texture->GetBrushColor(), ui_texture->GetCoordinate(), ui_texture->GetTilling(), pivot));

									if (widget->GetWidgetSpaceType() == eWidgetSpaceType::kBillboard)
										vertices.emplace_back(std::move(vertex));
									else if (widget->GetWidgetSpaceType() == eWidgetSpaceType::kFixUpBillboard)
										fix_up_vertices.emplace_back(std::move(vertex));
								}
							}
						}
					}

					widget->SetVisiblity(false);
				}
			}

			std::vector<PivotWidgetVertex> final_vertices;

			m_num_of_draw_widget_ui_data[0] = static_cast<UINT>(vertices.size());
			m_widget_start_vertex_locations[0] = 0;
			std::move(vertices.begin(), vertices.end(), std::back_inserter(final_vertices));

			m_num_of_draw_widget_ui_data[1] = static_cast<UINT>(fix_up_vertices.size());
			m_widget_start_vertex_locations[1] = static_cast<UINT>(final_vertices.size());
			std::move(fix_up_vertices.begin(), fix_up_vertices.end(), std::back_inserter(final_vertices));

			m_widget_primitive->UpdateVertices(final_vertices);
		}
	}

	void WidgetRenderItem::Draw(ID3D12GraphicsCommandList* command_list, eWidgetSpaceType type)
	{
		switch (type)
		{
		case client_fw::eWidgetSpaceType::kWorld:
			m_world_widget_primitive->PreDraw(command_list);
			m_world_widget_primitive->Draw(command_list, m_num_of_draw_world_widget_ui_data);
			break;
		case client_fw::eWidgetSpaceType::kBillboard:
			m_widget_primitive->PreDraw(command_list);
			m_widget_primitive->Draw(command_list,
				m_num_of_draw_widget_ui_data[0], m_widget_start_vertex_locations[0]);
			break;
		case client_fw::eWidgetSpaceType::kFixUpBillboard:
			m_widget_primitive->PreDraw(command_list);
			m_widget_primitive->Draw(command_list,
				m_num_of_draw_widget_ui_data[1], m_widget_start_vertex_locations[1]);
			break;
		case client_fw::eWidgetSpaceType::kScreen:
			m_widget_primitive->PreDraw(command_list);
			m_widget_primitive->Draw(command_list,
				m_num_of_draw_widget_ui_data[2], m_widget_start_vertex_locations[2]);
			break;
		default:
			break;
		}
		
	}

	void WidgetRenderItem::RegisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp)
	{
		const auto& ui_layer = widget_comp->GetUserInterfaceLayer();
		if (ui_layer != nullptr)
		{
			UINT num_of_data = ui_layer->GetNumOfVisibleTexture();
			if (widget_comp->GetWidgetSpaceType() == eWidgetSpaceType::kWorld)
			{
				widget_comp->SetRenderItemIndex(static_cast<UINT>(m_world_widget_components.size()));
				m_world_widget_components.push_back(widget_comp);

				if (m_num_of_world_widget_ui_data == 0)
				{
					m_num_of_world_widget_ui_data = num_of_data;
					m_real_num_of_world_widget_ui_data = num_of_data;
					m_is_need_world_widget_resource_create = true;
				}
				else
				{
					m_real_num_of_world_widget_ui_data += num_of_data;
					while (m_real_num_of_world_widget_ui_data > m_num_of_world_widget_ui_data)
					{
						m_num_of_world_widget_ui_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_world_widget_ui_data) * 1.5f));
						m_is_need_world_widget_resource_create = true;
					}
				}
			}
			else
			{
				widget_comp->SetRenderItemIndex(static_cast<UINT>(m_widget_components.size()));
				m_widget_components.push_back(widget_comp);

				if (m_num_of_widget_ui_data == 0)
				{
					m_num_of_widget_ui_data = num_of_data;
					m_real_num_of_widget_ui_data = num_of_data;
					m_is_need_widget_resource_create = true;
				}
				else
				{
					m_real_num_of_widget_ui_data += num_of_data;
					while (m_real_num_of_widget_ui_data > m_num_of_widget_ui_data)
					{
						m_num_of_widget_ui_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_widget_ui_data) * 1.5f));
						m_is_need_widget_resource_create = true;
					}
				}
			}
		}
	}

	void WidgetRenderItem::UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp)
	{
		const auto& ui_layer = widget_comp->GetUserInterfaceLayer();
		if (ui_layer != nullptr)
		{
			UINT index = widget_comp->GetRenderItemIndex();
			UINT num_of_data = ui_layer->GetNumOfVisibleTexture();

			if (widget_comp->GetWidgetSpaceType() == eWidgetSpaceType::kWorld)
			{
				std::iter_swap(m_world_widget_components.begin() + index, m_world_widget_components.end() - 1);
				m_world_widget_components[index]->SetRenderItemIndex(index);
				m_world_widget_components.pop_back();

				if (m_world_widget_components.empty())
				{
					m_num_of_world_widget_ui_data = 0;
					m_real_num_of_world_widget_ui_data = 0;
					m_is_need_world_widget_resource_create = true;
				}
			}
			else
			{
				std::iter_swap(m_widget_components.begin() + index, m_widget_components.end() - 1);
				m_widget_components[index]->SetRenderItemIndex(index);
				m_widget_components.pop_back();

				if (m_widget_components.empty())
				{
					m_num_of_widget_ui_data = 0;
					m_real_num_of_widget_ui_data = 0;
					m_is_need_widget_resource_create = true;
				}
			}
		}
	}

	bool WidgetRenderItem::IsDrawDataEmpty(eWidgetSpaceType type)
	{
		switch (type)
		{
		case client_fw::eWidgetSpaceType::kWorld:
		default:
			return m_num_of_draw_world_widget_ui_data == 0;
		case client_fw::eWidgetSpaceType::kBillboard:
			return m_num_of_draw_widget_ui_data[0] == 0;
		case client_fw::eWidgetSpaceType::kFixUpBillboard:
			return m_num_of_draw_widget_ui_data[1] == 0;
		case client_fw::eWidgetSpaceType::kScreen:
			return m_num_of_draw_widget_ui_data[2] == 0;
		}
	}

}
