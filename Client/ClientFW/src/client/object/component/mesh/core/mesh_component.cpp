#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/actor/core/actor.h"
#include "client/util/octree/octree_helper.h"

namespace client_fw
{
	MeshComponent::MeshComponent(const std::string& name, const std::string& draw_shader_name)
		: Component(name, 20), m_draw_shader_name(draw_shader_name)
	{
	}

	bool MeshComponent::Initialize()
	{
		bool result = true;
		if (m_mesh != nullptr)
		{
			RegisterToMeshOctree();
			result = RegisterToRenderSystem();
		}
		return result;
	}

	void MeshComponent::Shutdown()
	{
		if (m_mesh != nullptr)
		{
			UnregisterFromMeshOctree();
			UnregisterFromRenderSystem();
			m_mesh = nullptr;
		}
	}

	void MeshComponent::UpdateWorldMatrix()
	{
		if (m_owner.expired() == false)
		{
			m_mesh->GetOrientedBox().Transform(m_oriented_box, XMLoadFloat4x4(&m_owner.lock()->GetWorldMatrix()));
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

	void MeshComponent::RegisterToMeshOctree()
	{
		OctreeHelper::RegisterMeshComponent(shared_from_this());
	}

	void MeshComponent::UnregisterFromMeshOctree()
	{
		OctreeHelper::UnregisterMeshComponent(shared_from_this());
	}

	void MeshComponent::SetMesh(const std::string& file_path)
	{
		m_mesh = AssetStore::LoadMesh(file_path);
	}

	void MeshComponent::AddMeshTreeNode(const WPtr<MeshTreeNode>& tree_node)
	{
		m_mesh_tree_node.push_back(tree_node);
	}
}