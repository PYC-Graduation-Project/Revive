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
	}

	void OctreeManager::RegisterVisualOctrees(std::vector<SPtr<VisualOctree>>&& octrees)
	{
		m_visual_octree_manager->RegisterOctrees(std::move(octrees));
	}

	void OctreeManager::UnregisterOctrees()
	{
		m_visual_octree_manager->UnregisterOctrees();
	}

	VisualOctreeManager* VisualOctreeManager::s_instance = nullptr;

	VisualOctreeManager::VisualOctreeManager()
	{
		s_instance = this;
	}

	void VisualOctreeManager::RegisterOctrees(std::vector<SPtr<VisualOctree>>&& octrees)
	{
		UnregisterOctrees();
		m_visual_octrees = octrees;
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
					octree->RegisterMeshComponent(mesh);
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
			}
		}
	}
}
