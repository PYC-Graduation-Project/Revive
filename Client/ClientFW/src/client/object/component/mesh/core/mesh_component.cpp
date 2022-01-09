#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"
#include "client/asset/core/asset_store.h"

namespace client_fw
{
	MeshComponent::MeshComponent(const std::string& name)
		: RenderComponent(name, eRenderComponentType::kMesh)
	{
	}

	void MeshComponent::Initialize()
	{
		if (m_mesh != nullptr)
			RegisterToRenderSystem(eShaderType::kDefault);
	}

	void MeshComponent::Shutdown()
	{
		if (m_mesh != nullptr)
		{
			UnregisterFromRenderSystem(eShaderType::kDefault);
			m_mesh = nullptr;
		}
	}

	void MeshComponent::RegisterToRenderSystem(const std::string& shader_name)
	{
		Render::RegisterMeshComponent(std::static_pointer_cast<MeshComponent>(shared_from_this()), shader_name);
	}

	void MeshComponent::RegisterToRenderSystem(eShaderType shader_type)
	{
		Render::RegisterMeshComponent(std::static_pointer_cast<MeshComponent>(shared_from_this()), shader_type);
	}

	void MeshComponent::UnregisterFromRenderSystem(const std::string& shader_name)
	{
		Render::UnregisterMeshComponent(std::static_pointer_cast<MeshComponent>(shared_from_this()), shader_name);
	}

	void MeshComponent::UnregisterFromRenderSystem(eShaderType shader_type)
	{
		Render::UnregisterMeshComponent(std::static_pointer_cast<MeshComponent>(shared_from_this()), shader_type);
	}

	void MeshComponent::SetMesh(const std::string& file_path)
	{
		m_mesh = AssetStore::LoadMesh(file_path);
	}
}