#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	class Texture;

	class Material : public Asset
	{
	public:
		Material() = default;
		virtual ~Material() = default;

	private:
		UINT m_material_resource_index = 0;
		Vec4 m_base_color;

	public:
		UINT GetResourceIndex() const { return m_material_resource_index; }
		void SetResourceIndex(UINT index) { m_material_resource_index = index; }
		const Vec4& GetBaseColor() const { return m_base_color; }
		void SetBaseColor(const Vec4& base_color) { m_base_color = base_color; }

	private:
		SPtr<Texture> m_diffuse_texture;

	public:
		const SPtr<Texture>& GetDiffuseTexture() const { return m_diffuse_texture; }
		void SetDiffuseTexture(const SPtr<Texture>& texture) { m_diffuse_texture = texture; }

	};
}


