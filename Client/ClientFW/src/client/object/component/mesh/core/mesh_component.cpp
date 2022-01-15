#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"
#include "client/asset/core/asset_store.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	MeshComponent::MeshComponent(const std::string& name, const std::string& draw_shader_name)
		: Component(name), m_draw_shader_name(draw_shader_name)
	{
	}

	bool MeshComponent::Initialize()
	{
		bool result = true;
		if (m_mesh != nullptr)
			result = RegisterToRenderSystem();
		return result;
	}

	void MeshComponent::Shutdown()
	{
		if (m_mesh != nullptr)
		{
			UnregisterFromRenderSystem();
			m_mesh = nullptr;
		}
	}

	bool MeshComponent::RegisterToRenderSystem()
	{
		return Render::RegisterMeshComponent(shared_from_this(), m_draw_shader_name);
	}

	void MeshComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterMeshComponent(shared_from_this(), m_draw_shader_name);
	}

	void MeshComponent::SetMesh(const std::string& file_path)
	{
		m_mesh = AssetStore::LoadMesh(file_path);
	}
}