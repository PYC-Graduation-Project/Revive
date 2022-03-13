#pragma once
#include "client/object/component/render/billboard_component.h"

namespace client_fw
{
	class Material;

	class MaterialBillboardComponent : public BillboardComponent
	{
	//public:
	private:
		MaterialBillboardComponent(const std::string& name = "material billboard");
		virtual ~MaterialBillboardComponent() = default;

	protected:
		SPtr<Material> m_material;

	public:
		const SPtr<Material>& GetMaterial() const { return m_material; }
		void SetMaterial(const std::string& path);
		void SetMaterial(const SPtr<Material>& material) { m_material = material; }
	};

}

