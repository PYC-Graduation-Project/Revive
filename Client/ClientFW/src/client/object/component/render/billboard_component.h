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
		Vec2 m_coordinate;
		bool m_fix_up_vector;

	public:
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size);
		const Vec2& GetCoordinate() const { return m_coordinate; }
		void SetCoordinate(const Vec2& coord) { m_coordinate = coord; }
		bool IsFixUpVector() const { return m_fix_up_vector; }
		void SetFixUpVector(bool fix) { m_fix_up_vector = fix; }

	protected:
		SPtr<BillboardComponent> SharedFromThis();

	};

}


