#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/actor/core/actor.h"
#include "client/util/octree/octree_manager.h"
#include "client/physics/collision/mesh_collision_manager.h"

namespace client_fw
{
	MeshComponent::MeshComponent(const std::string& name, const std::string& draw_shader_name)
		: Component(name, 20), m_draw_shader_name(draw_shader_name)
	{
		m_oriented_box = CreateSPtr<BOrientedBox>();
	}

	bool MeshComponent::Initialize()
	{
		bool result = true;
		if (m_mesh != nullptr)
		{
			RegisterToMeshOctree();
			if (m_collision_manager != nullptr)
			{
				m_collision_manager->SetOwner(m_owner);
			}
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
			m_oriented_box->Transform(m_mesh->GetOrientedBox(), m_owner.lock()->GetWorldMatrix());
			Vec3 extents = m_oriented_box->GetExtents();
			m_max_extent = extents.x;
			m_max_extent = max(m_max_extent, extents.y);
			m_max_extent = max(m_max_extent, extents.z);
			if (m_collision_manager->GetCollisionInfo().preset != eCollisionPreset::kNoCollision)
				CollisionOctreeManager::GetOctreeManager().ReregisterMeshComponent(shared_from_this());
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
		VisualOctreeManager::GetOctreeManager().RegisterMeshComponent(shared_from_this());
	}

	void MeshComponent::UnregisterFromMeshOctree()
	{
		VisualOctreeManager::GetOctreeManager().UnregisterMeshComponent(shared_from_this());
		if (m_collision_manager->GetCollisionInfo().preset != eCollisionPreset::kNoCollision)
			CollisionOctreeManager::GetOctreeManager().UnregisterMeshComponent(shared_from_this());
	}

	void MeshComponent::SetMesh(const std::string& file_path)
	{
		m_mesh = AssetStore::LoadMesh(file_path);
		m_mesh->CreateCollision(shared_from_this());
	}

	void MeshComponent::SetMeshCollisionManager(SPtr<MeshCollisionManager>&& collision_manager)
	{
		m_collision_manager = std::move(collision_manager);
	}

	bool MeshComponent::IsUseLevelOfDetail() const
	{
		return m_mesh->GetLODCount() > 1;
	}

	void MeshComponent::SetLevelOfDetail(UINT lod)
	{
		m_level_of_detail = min(lod, m_mesh->GetLODCount() - 1);
		m_mesh->AddLODMeshCount(m_level_of_detail);
	}

	void MeshComponent::AddVisualTreeNode(const WPtr<VisualTreeNode>& tree_node)
	{
		m_visual_tree_node.push_back(tree_node);
	}

	void MeshComponent::AddCollisionTreeNode(const WPtr<CollisionTreeNode>& tree_node)
	{
		m_collision_tree_node.push_back(tree_node);
	}
}