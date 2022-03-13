#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	enum class eBillboardType
	{
		kTexture, kMaterial
	};

	class BillboardComponent : public RenderComponent
	{
	protected:
		BillboardComponent(const std::string& name, eBillboardType type);
		virtual ~BillboardComponent() = default;

		virtual void UpdateOrientedBox() override;

	private:
		eBillboardType m_type;

	protected:
		Vec2 m_size;
		bool m_fix_up_vector;

	public:
		eBillboardType GetBillboardType() const { return m_type; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size);
		bool IsFixUpVector() const { return m_fix_up_vector; }
		void SetFixUpVector(bool fix) { m_fix_up_vector = fix; }

	protected:
		SPtr<BillboardComponent> SharedFromThis();

	};

}


