#include "stdafx.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"
#include "client/object/component/render/billboard_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	BillboardComponent::BillboardComponent(const std::string& name)
		: RenderComponent(name, 20, eRenderType::kBillboard, Render::ConvertShaderType(eShaderType::kBillboard))
		, m_size(Vec2(100.f, 100.f))
	{
		m_oriented_box = CreateSPtr<BOrientedBox>();
	}

	void BillboardComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(), GetWorldMatrix());
		float extent = m_size.Length();
		m_oriented_box->SetExtents(Vec3(extent, extent, extent));
	}

	void BillboardComponent::SetTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
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
