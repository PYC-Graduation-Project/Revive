#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class Texture;

	class BillboardComponent : public RenderComponent
	{
	public:
		BillboardComponent(const std::string& name = "billboard component");
		virtual ~BillboardComponent() = default;

	protected:
		virtual void UpdateOrientedBox() override;

	protected:
		SPtr<Texture> m_texture;
		Vec2 m_size;

	public:
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size);

	protected:
		SPtr<BillboardComponent> SharedFromThis();

	};

}


