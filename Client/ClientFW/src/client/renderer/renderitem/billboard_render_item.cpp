#include "stdafx.h"
#include "client/renderer/renderitem/billboard_render_item.h"
#include "client/object/component/render/billboard_component.h"
#include "client/asset/primitive/billboard_primitive.h"
#include "client/asset/texture/texture.h"

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

	void BillboardRenderItem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		std::vector<BillboardVertex> vertices;

		for (const auto& bb_comp : m_billboard_components)
		{
			if (bb_comp->IsVisible() && bb_comp->GetTexture() != nullptr)
			{
				vertices.emplace_back(BillboardVertex(bb_comp->GetWorldPosition(),
					bb_comp->GetSize(), bb_comp->GetTexture()->GetResourceIndex(), Vec2(0.f, 0.f)));

				LOG_INFO(bb_comp->GetWorldPosition());
				LOG_INFO(bb_comp->GetSize());
				LOG_INFO(bb_comp->GetTexture()->GetResourceIndex());

				bb_comp->SetVisiblity(false);
			}
		}

		if (vertices.empty() == false)
		{
			m_num_of_draw_billboard_data = static_cast<UINT>(vertices.size());
			m_billboard_primitive->UpdateBillboardVertices(vertices);

			m_billboard_primitive->Draw(command_list, m_num_of_draw_billboard_data);
		}
	}

	void BillboardRenderItem::RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		bb_comp->SetRenderItemIndex(static_cast<UINT>(m_billboard_components.size()));

		m_billboard_components.push_back(bb_comp);

		if (m_num_of_billboard_data == 0)
		{
			m_num_of_billboard_data = 1;
			m_is_need_resource_create = true;
		}
		else if(m_num_of_billboard_data < static_cast<UINT>(m_billboard_components.size()))
		{
			m_num_of_billboard_data = static_cast<UINT>(roundf(static_cast<float>(m_num_of_billboard_data) * 1.5f));
			m_is_need_resource_create = true;
		}
	}

	void BillboardRenderItem::UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		UINT index = bb_comp->GetRenderItemIndex();

		std::iter_swap(m_billboard_components.begin() + index, m_billboard_components.end() - 1);
		m_billboard_components[index]->SetRenderItemIndex(index);
		m_billboard_components.pop_back();

		UINT size = static_cast<UINT>(roundf(static_cast<float>(m_num_of_billboard_data * 0.66f)));
		if (size >= m_billboard_components.size())
		{
			if (m_billboard_components.empty())
			{
				m_num_of_billboard_data = 0;
			}
			else
			{
				m_num_of_billboard_data = size;
				m_is_need_resource_create = true;
			}
		}
	}
}
