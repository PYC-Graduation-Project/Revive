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

	void MaterialBillboardComponent::SetMaterial(const std::string& mtl_path)
	{
		const auto& material = AssetStore::LoadMaterial(mtl_path);
		if (material != nullptr)
			SetMaterial(material);
		else
			LOG_ERROR("Could not find material : {0}", mtl_path);
	}

	void MaterialBillboardComponent::SetMaterial(const std::string& path, const std::string& mtl_name)
	{
		const auto& material = AssetStore::LoadMaterial(path, mtl_name);
		if (material != nullptr)
			SetMaterial(material);
		else
			LOG_ERROR("Could not find material : {0} - {1}", path, mtl_name);
	}
}