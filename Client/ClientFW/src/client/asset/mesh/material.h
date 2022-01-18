#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	class Material : public Asset
	{
	public:
		Material() = default;
		virtual ~Material() = default;

	private:
		UINT m_material_resource_index = 0;
		Vec4 m_base_color;

	public:
		const Vec4& GetBaseColor() const { return m_base_color; }
		void SetBaseColor(const Vec4& base_color) { m_base_color = base_color; }
	};
}


