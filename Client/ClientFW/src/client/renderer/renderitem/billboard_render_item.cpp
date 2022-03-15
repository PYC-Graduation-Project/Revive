#include "stdafx.h"
#include "client/renderer/renderitem/billboard_render_item.h"
#include "client/object/component/render/texture_billboard_component.h"
#include "client/object/component/render/material_billboard_component.h"
#include "client/asset/primitive/billboard_primitive.h"
#include "client/asset/texture/texture.h"
#include "client/asset/material/material.h"
#include "client/util/octree/octree.h"

namespace client_fw
{
	BillboardRenderItem::BillboardRenderItem()
	{
		m_billboard_primitive = CreateUPtr<BillboardPrimitive>();
	}

	BillboardRenderItem::~BillboardRenderItem()
	{
	}

	void BillboardRenderItem::Initialize(ID3D12Device* device)
	{
		m_billboard_primitive->Initialize(device);
	}

	void BillboardRenderItem::Shutdown()
	{
		m_billboard_primitive->Shutdown();
	}

	void BillboardRenderItem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_is_need_resource_create)
		{
			m_billboard_primitive->Update(device, m_num_of_billboard_data);
			m_is_need_resource_create = false;
		}
	}

	void BillboardRenderItem::Draw(ID3D12GraphicsCommandList* command_list, bool is_fix_up)
	{
		m_billboard_primitive->Draw(command_list,
			m_num_of_draw_billboard_data[static_cast<UINT>(is_fix_up)], m_start_vertex_locations[static_cast<UINT>(is_fix_up)]);
	}

	TextureBillboardRenderItem::TextureBillboardRenderItem()
		: BillboardRenderItem()
	{
	}

	TextureBillboardRenderItem::~TextureBillboardRenderItem()
	{
	}

	void TextureBillboardRenderItem::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		std::vector<BillboardVertex> vertices;
		std::vector<BillboardVertex> fix_up_vertices;

		for (const auto& bb_comp : m_billboard_components)
		{
			if (bb_comp->IsVisible() && bb_comp->GetTexture() != nullptr)
			{
				BillboardVertex vertex(BillboardVertex(bb_comp->GetWorldPosition(),
					bb_comp->GetSize(), bb_comp->GetTexture()->GetResourceIndex(),
					bb_comp->GetCoordinate()));

				if (bb_comp->IsFixUpVector())
					fix_up_vertices.emplace_back(std::move(vertex));
				else
					vertices.emplace_back(std::move(vertex));

				bb_comp->SetVisiblity(false);
			}
		}

		std::vector<BillboardVertex> final_vertices;
		m_num_of_draw_billboard_data[0] = static_cast<UINT>(vertices.size());
		m_start_vertex_locations[0] = 0;
		std::move(vertices.begin(), vertices.end(), std::back_inserter(final_vertices));

		m_num_of_draw_billboard_data[1] = static_cast<UINT>(fix_up_vertices.size());
		m_start_vertex_locations[1] = static_cast<UINT>(final_vertices.size());
		std::move(fix_up_vertices.begin(), fix_up_vertices.end(), std::back_inserter(final_vertices));

		if (final_vertices.empty() == false)
		{
			m_billboard_primitive->UpdateBillboardVertices(final_vertices);
			m_billboard_primitive->PreDraw(command_list);
		}
	}

	void TextureBillboardRenderItem::RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		if (bb_comp->GetBillboardType() == eBillboardType::kTexture)
		{
			bb_comp->SetRenderItemIndex(static_cast<UINT>(m_billboard_components.size()));
			m_billboard_components.push_back(std::static_pointer_cast<TextureBillboardComponent>(bb_comp));

			if (m_num_of_billboard_data == 0)
			{
				m_num_of_billboard_data = 1;
				m_is_need_resource_create = true;
			}
			else if (m_num_of_billboard_data < static_cast<UINT>(m_billboard_components.size()))
			{
				m_num_of_billboard_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_billboard_data) * 1.5f));
				m_is_need_resource_create = true;
			}
		}
		else
		{
			LOG_WARN("Could not register {0} to material billboard render item", bb_comp->GetName());
		}
	}

	void TextureBillboardRenderItem::UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		UINT index = bb_comp->GetRenderItemIndex();

		if (bb_comp->GetBillboardType() == eBillboardType::kTexture)
		{
			std::iter_swap(m_billboard_components.begin() + index, m_billboard_components.end() - 1);
			m_billboard_components[index]->SetRenderItemIndex(index);
			m_billboard_components.pop_back();

			UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_billboard_data * 0.66f)));
			if (size >= m_billboard_components.size())
			{
				if (m_billboard_components.empty())
				{
					m_num_of_billboard_data = 1;
					m_is_need_resource_create = true;
				}
				else
				{
					m_num_of_billboard_data = size;
					m_is_need_resource_create = true;
				}
			}
		}
	}

	MaterialBillboardRenderItem::MaterialBillboardRenderItem()
		: BillboardRenderItem()
	{
	}

	MaterialBillboardRenderItem::~MaterialBillboardRenderItem()
	{
	}

	void MaterialBillboardRenderItem::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		std::vector<BillboardVertex> vertices;
		std::vector<BillboardVertex> fix_up_vertices;

		for (const auto& bb_comp : m_billboard_components)
		{
			if (bb_comp->IsVisible() && bb_comp->GetMaterial() != nullptr)
			{
				BillboardVertex vertex(BillboardVertex(bb_comp->GetWorldPosition(),
					bb_comp->GetSize(), bb_comp->GetMaterial()->GetResourceIndex(),
					Vec2(0.0f, 0.0f)));

				if (bb_comp->IsFixUpVector())
					fix_up_vertices.emplace_back(std::move(vertex));
				else
					vertices.emplace_back(std::move(vertex));

				bb_comp->SetVisiblity(false);
			}
		}

		std::vector<BillboardVertex> final_vertices;
		m_num_of_draw_billboard_data[0] = static_cast<UINT>(vertices.size());
		m_start_vertex_locations[0] = 0;
		std::move(vertices.begin(), vertices.end(), std::back_inserter(final_vertices));

		m_num_of_draw_billboard_data[1] = static_cast<UINT>(fix_up_vertices.size());
		m_start_vertex_locations[1] = static_cast<UINT>(final_vertices.size());
		std::move(fix_up_vertices.begin(), fix_up_vertices.end(), std::back_inserter(final_vertices));

		if (final_vertices.empty() == false)
		{
			m_billboard_primitive->UpdateBillboardVertices(final_vertices);
			m_billboard_primitive->PreDraw(command_list);
		}
	}

	void MaterialBillboardRenderItem::RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		if (bb_comp->GetBillboardType() == eBillboardType::kMaterial)
		{
			bb_comp->SetRenderItemIndex(static_cast<UINT>(m_billboard_components.size()));
			m_billboard_components.push_back(std::static_pointer_cast<MaterialBillboardComponent>(bb_comp));

			if (m_num_of_billboard_data == 0)
			{
				m_num_of_billboard_data = 1;
				m_is_need_resource_create = true;
			}
			else if (m_num_of_billboard_data < static_cast<UINT>(m_billboard_components.size()))
			{
				m_num_of_billboard_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_billboard_data) * 1.5f));
				m_is_need_resource_create = true;
			}
		}
		else
		{
			LOG_WARN("Could not register {0} to material billboard render item", bb_comp->GetName());
		}
	}

	void MaterialBillboardRenderItem::UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		UINT index = bb_comp->GetRenderItemIndex();

		if (bb_comp->GetBillboardType() == eBillboardType::kMaterial)
		{
			std::iter_swap(m_billboard_components.begin() + index, m_billboard_components.end() - 1);
			m_billboard_components[index]->SetRenderItemIndex(index);
			m_billboard_components.pop_back();

			UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_billboard_data * 0.66f)));
			if (size >= m_billboard_components.size())
			{
				if (m_billboard_components.empty())
				{
					m_num_of_billboard_data = 1;
					m_is_need_resource_create = true;
				}
				else
				{
					m_num_of_billboard_data = size;
					m_is_need_resource_create = true;
				}
			}
		}
	}
}
