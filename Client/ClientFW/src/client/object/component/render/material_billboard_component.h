#pragma once
#include "client/object/component/render/billboard_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	class Material;

	class MaterialBillboardComponent : public BillboardComponent
	{
	public:
	//private:
		MaterialBillboardComponent(const std::string& name = "material billboard",
			const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kOpaqueMaterialBillboard));
		virtual ~MaterialBillboardComponent() = default;

	protected:
		SPtr<Material> m_material;

	public:
		const SPtr<Material>& GetMaterial() const { return m_material; }

		//if .mtl file (parent releative path + mtl's newmtl name)
		void SetMaterial(const std::string& mtl_path);

		//if .mtl file (path = releative path, mtl_name = mtl's newmtl name)
		void SetMaterial(const std::string& path, const std::string& mtl_name);
		void SetMaterial(const SPtr<Material>& material) { m_material = material; }
	};

}

