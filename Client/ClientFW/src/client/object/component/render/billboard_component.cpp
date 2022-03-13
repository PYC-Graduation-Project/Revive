#include "stdafx.h"
#include "client/object/component/render/billboard_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	BillboardComponent::BillboardComponent(const std::string& name, eBillboardType type)
		: RenderComponent(name, 20, eRenderType::kBillboard, Render::ConvertShaderType(eShaderType::kBillboard))
		, m_type(type), m_size(Vec2(100.f, 100.f))
	{
		m_oriented_box = CreateSPtr<BOrientedBox>();
	}

	void BillboardComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(), GetWorldMatrix());
		float extent = m_size.Length();
		m_oriented_box->SetExtents(Vec3(extent, extent, extent));
	}

	void BillboardComponent::SetSize(const Vec2& size)
	{
		m_size = size;
		UpdateOrientedBox();
	}

	SPtr<BillboardComponent> BillboardComponent::SharedFromThis()
	{
		return std::static_pointer_cast<BillboardComponent>(shared_from_this());
	}
}
