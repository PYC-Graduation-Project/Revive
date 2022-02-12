#include "stdafx.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/mesh_octree.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	OctreeManager::OctreeManager()
	{
		m_visual_octree_manager = CreateUPtr<VisualOctreeManager>();
		m_collision_octree_manager = CreateUPtr<CollisionOctreeManager>();
	}

	void OctreeManager::RegisterVisualOctrees(std::vector<SPtr<VisualOctree>>&& octrees)
	{
		m_visual_octree_manager->RegisterOctrees(std::move(octrees));
	}

	void OctreeManager::RegisterCollisionOctrees(std::vector<SPtr<CollisionOctree>>&& octrees)
	{
		m_collision_octree_manager->RegisterOctrees(std::move(octrees));
	}

	void OctreeManager::UnregisterOctrees()
	{
		m_visual_octree_manager->UnregisterOctrees();
		m_collision_octree_manager->UnregisterOctrees();
	}

	VisualOctreeManager* VisualOctreeManager::s_instance = nullptr;

	VisualOctreeManager::VisualOctreeManager()
	{
		s_instance = this;
	}

	void VisualOctreeManager::RegisterOctrees(std::vector<SPtr<VisualOctree>>&& octrees)
	{
		UnregisterOctrees();
		for (auto&& octree : octrees)
		{
			if (octree->GetWidth() > 0.0f)
				m_visual_octrees.emplace_back(std::move(octree));
		}
		m_is_active = true;
		for (const auto& octree : m_visual_octrees)
			octree->Initialize();
	}

	void VisualOctreeManager::UnregisterOctrees()
	{
		for (const auto& octree : m_visual_octrees)
			octree->Shutdown();
		m_visual_octrees.clear();
		m_movable_meshes.clear();
		m_is_active = false;
	}

	void VisualOctreeManager::RegisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		if (m_is_active)
		{
			if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
			{
				m_movable_meshes.push_back(mesh);
			}
			else
			{
				for (const auto& octree : m_visual_octrees)
				{
					const auto& root_node = octree->GetRootNode();
					if (mesh->GetOrientedBox()->Intersects(root_node->bounding_box))
						octree->RegisterMeshComponent(mesh, root_node);
				}
			}
		}
	}

	void VisualOctreeManager::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh)
	{
		if (m_is_active)
		{
			auto UnregisterMesh([&mesh](std::vector<SPtr<MeshComponent>>& meshes) {
				auto iter = std::find(meshes.begin(), meshes.end(), mesh);
				if (iter != meshes.end())
				{
					std::iter_swap(iter, meshes.end() - 1);
					meshes.pop_back();
				}
				});

			if (mesh->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
			{
				UnregisterMesh(m_movable_meshes);
			}
			else
			{
				for (const auto& tree_node : mesh->GetVisualTreeNodes())
					UnregisterMesh(tree_node.lock()->mesh_components);
				mesh->ResetVisualTreeNode();
			}
		}
	}

	CollisionOctreeManager* CollisionOctreeManager::s_instance = nullptr;

	CollisionOctreeManager::CollisionOctreeManager()
	{
		s_instance = this;
	}

	void CollisionOctreeManager::RegisterOctrees(std::vector<SPtr<CollisionOctree>>&& octrees)
	{
		UnregisterOctrees();
		for (auto&& octree : octrees)
		{
			if (octree->GetWidth() > 0.0f)
				m_collision_octrees.emplace_back(std::move(octree));
		}
		m_is_active = true;
		for (const auto& octree : m_collision_octrees)
			octree->Initialize();
	}

	void CollisionOctreeManager::UnregisterOctrees()
	{
		for (const auto& octree : m_collision_octrees)
			octree->Shutdown();
		m_collision_octrees.clear();
		m_is_active = false;
	}

	void CollisionOctreeManager::RegisterSceneComponent(const SPtr<SceneComponent>& mesh)
	{
		if (m_is_active)
		{
			for (const auto& octree : m_collision_octrees)
			{
				const auto& root_node = octree->GetRootNode();
				if (mesh->GetOrientedBox()->Intersects(root_node->bounding_box))
					octree->RegisterSceneComponent(mesh, root_node);
			}
		}
	}

	void CollisionOctreeManager::UnregisterSceneComponent(const SPtr<SceneComponent>& scene_comp)
	{
		if (m_is_active)
		{
			auto UnregisterSceneComp([&scene_comp](std::vector<SPtr<SceneComponent>>& scene_comps) {
				auto iter = std::find(scene_comps.begin(), scene_comps.end(), scene_comp);
				if (iter != scene_comps.end())
				{
					std::iter_swap(iter, scene_comps.end() - 1);
					scene_comps.pop_back();
				}
				});

			for (const auto& tree_node : scene_comp->GetCollisionTreeNodes())
			{
				if(scene_comp->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
					UnregisterSceneComp(tree_node.lock()->movable_scene_components);
				else 
					UnregisterSceneComp(tree_node.lock()->static_scene_components);
				scene_comp->ResetCollisionTreeNode();
			}
		}
	}

	void CollisionOctreeManager::ReregisterSceneComponent(const SPtr<SceneComponent>& scene_comp)
	{
		if (m_is_active)
		{
			const auto& tree_nodes = scene_comp->GetCollisionTreeNodes();
			ContainmentType type;

			if (tree_nodes.size() == 1)
			{
				//Contain from beginning
				type = tree_nodes[0].lock()->bounding_box.Contains(*scene_comp->GetOrientedBox());

				if (type != ContainmentType::CONTAINS)
				{
					UnregisterSceneComponent(scene_comp);
					RegisterSceneComponent(scene_comp);
				}
			}
			else
			{
				UnregisterSceneComponent(scene_comp);
				RegisterSceneComponent(scene_comp);
			}			
		}
	}
}
