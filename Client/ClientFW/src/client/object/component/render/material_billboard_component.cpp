#include "stdafx.h"
#include "client/object/component/render/material_billboard_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/material/material.h"

namespace client_fw
{
	MaterialBillboardComponent::MaterialBillboardComponent(const std::string& name,
		const std::string& draw_shader_name)
		: BillboardComponent(name, eBillboardType::kMaterial, draw_shader_name)
	{
	}

	void MaterialBillboardComponent::SetMaterial(const std::string& path)
	{
		const auto& material = AssetStore::LoadMaterial(path);
		if (material != nullptr)
			SetMaterial(material);
		else
			LOG_WARN("Could not find material : {0}", path);
	}
}