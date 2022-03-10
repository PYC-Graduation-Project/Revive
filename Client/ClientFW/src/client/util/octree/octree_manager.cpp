#include "stdafx.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/octree.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/core/bounding_mesh.h"
#include "client/physics/collision/collisioner/collisioner.h"
#include "client/physics/collision/collision_util.h"

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
		m_movable_render_comps.clear();
		m_is_active = false;
	}

	void VisualOctreeManager::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp)
	{
		if (m_is_active)
		{
			if (render_comp->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
			{
				m_movable_render_comps.push_back(render_comp);
			}
			else
			{
				for (const auto& octree : m_visual_octrees)
				{
					const auto& root_node = octree->GetRootNode();
					if (render_comp->GetOrientedBox()->Intersects(root_node->bounding_box))
						octree->RegisterRenderComponent(render_comp, root_node);
				}
			}
		}
	}

	void VisualOctreeManager::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp)
	{
		if (m_is_active)
		{
			auto UnregisterRenderComp([&render_comp](std::vector<SPtr<RenderComponent>>& render_comps) {
				auto iter = std::find(render_comps.begin(), render_comps.end(), render_comp);
				if (iter != render_comps.end())
				{
					std::iter_swap(iter, render_comps.end() - 1);
					render_comps.pop_back();
				}
				});

			if (render_comp->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
			{
				UnregisterRenderComp(m_movable_render_comps);
			}
			else
			{
				for (const auto& tree_node : render_comp->GetVisualTreeNodes())
					UnregisterRenderComp(tree_node.lock()->render_components);
				render_comp->ResetVisualTreeNode();
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
				if (scene_comp->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
					UnregisterSceneComp(tree_node.lock()->movable_scene_components.at(scene_comp->GetCollisioner()->GetCollisionInfo().collision_type));
				else
					UnregisterSceneComp(tree_node.lock()->static_scene_components.at(scene_comp->GetCollisioner()->GetCollisionInfo().collision_type));
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
